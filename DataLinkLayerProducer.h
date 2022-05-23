
const int MAX_CHAR = 64, MAX_CHAR_BIT = 512;

char * DecimalToBinary(int num) {
    /** This is for converting a decimal to binary, It helps the producer to convert
     * the control digit its 8 bits binary equivalence*/
    int n = num;
    char *s = malloc(sizeof(int) * 8);
    int i, c = 0;

    for (i = sizeof(int) * 2 - 1; i >= 0; i--) {
        n = num >> i;
        *(s + c) = (n & 1) ? '1' : '0';
        c++;
    }
    *(s + c) = NULL;
    return s;
}

char* framing(char * parityBitString, int BitsLeft){
    /** Generates frames for transmission by concatenating the syn bits, control bits, and data bits*/
    char * synBit = "0001011000010110";
    char * controlChar = (BitsLeft >= MAX_CHAR_BIT) ? DecimalToBinary(MAX_CHAR) : DecimalToBinary(
            MAX_CHAR_BIT % BitsLeft);
    char * frame;
    frame = (char *) malloc(sizeof(char) * (strlen(synBit) + strlen(controlChar) + strlen(parityBitString)) + 1);
    strcat(strcat(strcat(frame, synBit), controlChar), parityBitString);
    return frame;
}

char * DecimalToBinary2(unsigned long num) {
    /** This is for converting a decimal to binary, It helps the producer to convert
     * the control digit its 8 bits binary equivalence*/
    int n = num;
    char *s = malloc(sizeof(int) * 8);
    int i, c = 0;

    for (i = 31; i >= 0; i--) {
        n = num >> i;
        *(s + c) = (n & 1) ? '1' : '0';
        c++;
    }
    *(s + c) = NULL;
    return s;
}

char * framingWithCrc(char * bitToAddCrc) {
    char * bitsWithCrc = malloc(sizeof(char) * (strlen(bitToAddCrc) + 33)); //32 for reminder crc32 + 1 for null
    unsigned long crc = crc32(0, (const unsigned char *) bitToAddCrc, strlen(bitToAddCrc));
    char *crc2 = DecimalToBinary2(crc);
    strcat(strcat(bitsWithCrc, bitToAddCrc), crc2);
    return bitsWithCrc;
}

char* addHammingBits(const char* bitsBeforeHamming){
    char * messageBitsWithParity = (char *)malloc(523); //523 = 512 for data + 10 for parity + 1 null
    char static bitsAfterHamming[523]={}; //523 = 512 for data + 10 for parity + 1 null
    int noOfParityBitsToAdd = 0;
    int pos = 0, position;
    int bitSize = (int)strlen(bitsBeforeHamming);

    //calculate the number of parity bits needed. 2^p >= m + p + 1;
    while(bitSize > (int)pow(2, pos) - (pos + 1)) {
        noOfParityBitsToAdd++;
        pos++;
    }

    //The array is filled with redundant bits in the parity positions. ie positions corresponding to the power of 2
    int positionOfParityBits = 0; int positionOfNonParityBits =0;
    for(int i=0; i < noOfParityBitsToAdd + bitSize; i++){
        if(i == ((int)pow(2, positionOfParityBits) - 1)) {
            bitsAfterHamming[i] = 'p';  // assigning p to the indexes of powers of two
            positionOfParityBits++;
        }
        else{
            bitsAfterHamming[i] = bitsBeforeHamming[positionOfNonParityBits];
            positionOfNonParityBits++;
        }
    }

    //checking even parity and assigning the relevent bit to the indexes of powers of two
    for(int i = 0; i < noOfParityBitsToAdd; i++) {
        position = (int)pow(2,i);
        int positionIndex, count = 0;
        positionIndex = position - 1;
        while(positionIndex < noOfParityBitsToAdd + bitSize){
            for(int j = positionIndex; j < positionIndex + position; j++){
                if(bitsAfterHamming[j] == '1') count++;
            }
            positionIndex += 2 * position;
        }

        if(count%2 == 0)
            bitsAfterHamming[position - 1] = '0';
        else
            bitsAfterHamming[position - 1] = '1';
    }
    strcpy(messageBitsWithParity, bitsAfterHamming);
    return messageBitsWithParity;
}

