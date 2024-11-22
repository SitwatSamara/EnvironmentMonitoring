#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <cjson/cJSON.h>
#include "api_handler.h"

// Define the API URL and your API key
#define API_URL "http://api.openweathermap.org/data/2.5/weather?q=Islamabad&appid=540573637bcefd337875446d40332b36"

// Structure to store raw response data
struct MemoryStruct {
    char *memory;
    size_t size;
};

// Callback function to handle incoming data from libcurl
static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp) {
    size_t realsize = size * nmemb;
    struct MemoryStruct *mem = (struct MemoryStruct *)userp;

    // Reallocate memory to accommodate new data
    char *ptr = realloc(mem->memory, mem->size + realsize + 1);
    if (ptr == NULL) {
        printf("Not enough memory to allocate buffer\n");
        return 0;
    }
    mem->memory = ptr;

    // Copy data to buffer and update its size
    memcpy(&(mem->memory[mem->size]), contents, realsize);
    mem->size += realsize;
    mem->memory[mem->size] = 0;

    return realsize;
}

// Function to retrieve weather data
int retrieveData(EnvironmentalData *data) {
    CURL *curl;
    CURLcode res;
    struct MemoryStruct chunk = { .memory = malloc(1), .size = 0 };

    // Initialize libcurl
    curl_global_init(CURL_GLOBAL_ALL);
    curl = curl_easy_init();

    if (curl) {
        // Set libcurl options
        curl_easy_setopt(curl, CURLOPT_URL, API_URL);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);

        // Perform the request
        res = curl_easy_perform(curl);

        if (res == CURLE_OK) {
            // Save raw data to file
            printf("Saving data into a raw file..\n");
            FILE *raw_file = fopen("raw_data.json", "a+");
            if (raw_file != NULL) {
                fprintf(raw_file, "%s", chunk.memory);
                fprintf(raw_file,"\n");
                fclose(raw_file);
            }

            // Parse and process the JSON response
            printf("Processing data....\n");
            cJSON *json = cJSON_Parse(chunk.memory);
            if (json) {
                cJSON *main = cJSON_GetObjectItem(json, "main");
                if (main) {
                    cJSON *temp = cJSON_GetObjectItem(main, "temp");

                    if (temp ) {
                        data->temperature = temp->valuedouble - 273.15; // Convert Kelvin to Celsius

                        // Save processed data to file
                        printf("Saving processed data into a processed file...\n");
                        FILE *processed_file = fopen("processed_data.txt", "a+");
                        printf("The current temperature is %.2f°C\n",data->temperature);
                        
                        if (processed_file != NULL) {
                            fprintf(processed_file,  "%.2f\n", data->temperature);
                            // fprintf(processed_file,"\n");
                            fclose(processed_file);
                        }
                    }
                }
                cJSON_Delete(json);
            }
        } else {
            fprintf(stderr, "Failed to retrieve data: %s\n", curl_easy_strerror(res));
            free(chunk.memory);
            curl_easy_cleanup(curl);
            curl_global_cleanup();
            return 1;
        }

        // Cleanup
        curl_easy_cleanup(curl);
        free(chunk.memory);
        curl_global_cleanup();
    }
    return 0;
}
