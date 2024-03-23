
/*
 * The feature process is relatively independent. wav is usually used as input data. show init wav_splitted for 0.
 * and the general C notation makes it have good cross-platform and cross-machine compatibility.
 *
 * Author:   Jiahao Cui
 * UUN:      S2602811
 * Data:     24 May 2023
 * Email:    parker_cuijiahao@163.com
 * Version:  1.0
 */

#include <stdint.h>

#include "feature.h"

#define MAX_FRAME_SIZE_IN_SAMPLE        (1024)
#define MAX_MIC_NUMBER                  (2)



int main(int argc, char **argv) {
    
    int16_t wav_splitted[MAX_FRAME_SIZE_IN_SAMPLE * MAX_MIC_NUMBER] = {0};
    int16_t feature_data[1*5*64] = {0};
    
    isr_feature_process(wav_splitted, feature_data);
    
    return 0;
}