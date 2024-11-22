#include <stdio.h>
#include <unistd.h>
#include "api_handler.h"
#include "alerts.h"
#include <stdbool.h>

// Threshold for triggering an alert (example: temperature threshold)
#define TEMP_THRESHOLD 35.00

int main() {
    EnvironmentalData data;
    int i=0;

    printf("Retrieving data...\n"); 
    if (retrieveData(&data) == 0) {

        // Check for alert conditions
        printf("Checking for threshold...\n");
        if (data.temperature > TEMP_THRESHOLD) {
            printf("Sending Notification..\n");
            const char* title="Temperature Alert\n";
            const char* message= "Temperature has exceeded from its range..\n";
            sendAlert(title, message);
            printf("Alert sent successfully!\n");
            }
        }
        printf("waiting...\n");
    return 0;
}
