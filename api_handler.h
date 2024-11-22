#ifndef API_HANDLER_H
#define API_HANDLER_H

typedef struct {
    float temperature;
} EnvironmentalData;

int retrieveData(EnvironmentalData *data);

#endif
