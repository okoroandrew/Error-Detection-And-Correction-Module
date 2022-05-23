#include<stdlib.h>
#include<string.h>
#include "PhysicalLayerProducer.h"
#include "DataLinkLayerProducer.h"
#include "ApplicationLayerProducer.h"

#define MAX_CHAR_BITS 512
#define CHAR_BITS 8


char * mainProducer(char * inputFilePath, char * binaryOutputFilePath, int errorTechnique){

    char *text = readFile(inputFilePath);
    char *str = stringToBinary(text);

    /** Slice through the entire binary string,taking 7 bits at a time, add a parity bit in front, and store
     * in bitsToFrame */
    char * bitsToFrame = (char *) malloc(sizeof (char) * strlen(str) + 1);
    int start = 0, end = CHAR_BITS - 2;

    for (int i = 0; i < (strlen(str) + 1) / (CHAR_BITS - 1); i++){
        char * slicedBitsWithParity = addParityBit( sliceString(str, start, end));
        strcat(bitsToFrame, slicedBitsWithParity);
        start += CHAR_BITS - 1; end += CHAR_BITS - 1;
    }

    /** iterate through bitsToFrame, after every 512 bits (64 characters), frame, and store in writeThis.
     * transmit writeThis to a binary (.binf) file */
    char *toFrame; char * toTransmit;
    char * writeThis = (char *)malloc(sizeof (char) * 8056);
    int begin = 0, stop = MAX_CHAR_BITS - 1, countBitsLeft = (int)strlen(bitsToFrame);
    for (int i = 0; i <= strlen(bitsToFrame) / MAX_CHAR_BITS; i++){
        if(countBitsLeft >= MAX_CHAR_BITS) {
            toFrame = sliceString(bitsToFrame, begin, stop);
        }
        else if (countBitsLeft < MAX_CHAR_BITS) {
            toFrame = sliceString(bitsToFrame, begin, (int)strlen(bitsToFrame)-1);
        }

        //CRC done on whole frame
        if (errorTechnique == 1 || errorTechnique == 3) {
            toTransmit = framingWithCrc(framing(toFrame, countBitsLeft));
        }
        //Hamming Code
        else if (errorTechnique == 2 || errorTechnique == 4){
            toTransmit = framing(addHammingBits(toFrame), countBitsLeft);
        }
        strcat(writeThis, toTransmit);
        begin += MAX_CHAR_BITS; stop += MAX_CHAR_BITS; countBitsLeft -= MAX_CHAR_BITS;

    }
    writeFile(binaryOutputFilePath,  writeThis);
    return writeThis;
}


