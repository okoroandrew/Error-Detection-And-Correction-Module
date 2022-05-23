#include "DataLinkLayerConsumer.h"
#include "ApplicationLayerConsumer.h"
#define MAX_BIT_CRC 568
#define MAX_BIT_HAMMING 546

void mainConsumer(char * binaryData, char * outputFilePath, int errorTechnique, int bitToFlip, int bitToFlip2) {
    /**
     * Iterate through the binary data string, extract each frame using the syn bit, deframe by dropping
     * the syn bits and control bits, break the data bits into 8, check for parity and replace the MSB with
     * '0' after which it would be ready for conversion to text
     * */
    char* frameString; char* frameString1; char *frameString2;char * remainderMessage;
    char * ToTextConvert; char *ToTextConverter;

    if (errorTechnique == 1 || errorTechnique == 3) {
        /**
         * ErrorTechnique = 1 is uses CRC code without the error module
         * ErrorTechnique = 3 is uses CRC code with the error creation module adding a single-bit error
         * */
        int start = 0, end = MAX_BIT_CRC - 1, countBitsLeft = (int) strlen(binaryData);
        ToTextConverter = (char *) malloc(sizeof(char) * strlen(binaryData)); //This space is much

        for (int i = 0; i <= (strlen(binaryData) / MAX_BIT_CRC); i++) {
            if (countBitsLeft >= MAX_BIT_CRC) {
                frameString1 = sliceStringC(binaryData, start, end);
                frameString = sliceStringC(frameString1, 0, 535);
                if (errorTechnique == 3){
                    frameString = creatingError(bitToFlip, frameString); //adding error to a frame
                }
                remainderMessage = sliceStringC(frameString1, 536, 567);
            }
            else {
                frameString1 = sliceStringC(binaryData, start, (int)strlen(binaryData) - 1);
                frameString = sliceStringC(frameString1, 0, (int)strlen(frameString1) - 33);
                remainderMessage = sliceStringC(frameString1, (int)strlen(frameString1) - 32, (int)strlen(frameString1) - 1);
            }

            if (strcmp(crcErrorDetection(frameString), remainderMessage) == 0) {
                printf("No Error in frame %d\n", i + 1);
                ToTextConvert = getDataString(deFraming(frameString));
                strcat(ToTextConverter, ToTextConvert);
                start += MAX_BIT_CRC;
                end += MAX_BIT_CRC;
                countBitsLeft -= MAX_BIT_CRC;
            } else {
                printf("Error in frame %d\n", i + 1);
                start += MAX_BIT_CRC; end += MAX_BIT_CRC; countBitsLeft -= MAX_BIT_CRC;
            }
        }
    }

    else if (errorTechnique == 2 || errorTechnique == 4 || errorTechnique == 5){
        /**
         * ErrorTechnique = 2 is uses hamming code without the error module
         * ErrorTechnique = 4 is uses hamming code with the error creation module adding a single-bit error
         * ErrorTechnique = 5 is uses hamming code with the error creation module adding a two-bit error
         * */
        int start = 0, end = MAX_BIT_HAMMING - 1, countBitsLeft = (int) strlen(binaryData);
        ToTextConverter = (char *) malloc(sizeof(char) * strlen(binaryData));
        for (int i = 0; i <= strlen(binaryData) / MAX_BIT_HAMMING; i++) {
            if (countBitsLeft >= MAX_BIT_HAMMING) {
                frameString1 = sliceStringC(binaryData, start, end);
            }
            else {
                frameString1 = sliceStringC(binaryData, start, start + countBitsLeft - 1);
            }
            frameString2 = deFraming(frameString1);
            if (errorTechnique == 4) {
                frameString2 = creatingError(bitToFlip, frameString2); //adding a single bit error to each frame
            }
            else if (errorTechnique == 5) {
                printf("Error Detected\n");
                printf("Hamming code cannot correct more that one bit error\n");
                frameString2 = creatingError2(bitToFlip, bitToFlip2, frameString2); //adding 2 bits error to each frame
            }
            frameString = hammingErrorDetectionAndCorrection(frameString2);
            ToTextConvert = getDataString(frameString);
            strcat(ToTextConverter, ToTextConvert);
            start += MAX_BIT_HAMMING; end += MAX_BIT_HAMMING; countBitsLeft -= MAX_BIT_HAMMING;
        }
    }

    BinaryToText(ToTextConverter, outputFilePath);
    free(ToTextConverter);
}


