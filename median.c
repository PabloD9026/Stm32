#include "median.h"

void median_filter_init(median_filter_t* filter) {
    memset(filter->data, 0, sizeof(filter->data));
    memset(filter->sorted, 0, sizeof(filter->sorted));
    filter->index = 0;
}

int16_t median_filter_update(median_filter_t* filter, int16_t value) {
    uint8_t i, j;

    // Add the new value to the circular buffer
    filter->data[filter->index] = value;

    // Insert the new value into the sorted list
    for (i = 0; i < BUFFER_SIZE; i++) {
        if (filter->sorted[i] == filter->data[(filter->index + 1) % BUFFER_SIZE]) {
            break;
        }
    }
    filter->sorted[i] = value;

    // Sort the list
    for (i = 0; i < BUFFER_SIZE - 1; i++) {
        for (j = i + 1; j < BUFFER_SIZE; j++) {
            if (filter->sorted[i] > filter->sorted[j]) {
                int16_t temp = filter->sorted[i];
                filter->sorted[i] = filter->sorted[j];
                filter->sorted[j] = temp;
            }
        }
    }

    // Update the circular buffer index
    filter->index = (filter->index + 1) % BUFFER_SIZE;

    // Return the median value
    return filter->sorted[BUFFER_SIZE / 2];
}

