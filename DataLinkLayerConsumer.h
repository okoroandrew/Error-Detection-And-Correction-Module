
char *sliceStringC(char *str, int start, int end){
    /** This is for getting a substring from a string by specifying the parameters - string,
     * where the substring begins (start), and where it should (end)*/
    int i;
    int size = (end - start) + 2;
    char *bitString = (char *)malloc(size * sizeof(char));

    for (i = 0; start <= end; start++, i++){
        bitString[i] = str[start];
    }
    bitString[size] = '\0';
    return bitString;
}

char * parityCheck(char * dataString) {
    /** counts the number of 1 in a string bits and return even or odd based on the count*/
    int count = 0;
    for (char *dataStringP = dataString; *dataStringP != '\0'; ++dataStringP){
        if (*dataStringP == '1') count ++;
    }
    return (count % 2 == 0) ? "even parity" : "odd parity";
}

char * deFraming(char * frameString){
    /** from dropping the syn bits and control bits from a frame and returns only the data bits (with parity)*/
    char * syncBit = "0001011000010110";
    if (strcmp(sliceStringC(frameString, 0, 15), syncBit) == 0){
        return sliceStringC(frameString, 24, (int) strlen(frameString));
    }
    else if (strlen(frameString) == 0){
        printf("No frame received\n");
    }
    else printf("error in deFraming, check frame!!!\n");
    return NULL;
}

char * getDataString(char * deFramedString){
    /** Checks for parity,If the parity is odd, it drops the most significant bit and replaces it with a zero.
     * Otherwise, it would report it as a transmission error. All the data bit is merged
     * and ready for conversion into ASCII.*/
    char * dataBits = (char *)malloc(sizeof(char) * strlen(deFramedString) + 1);
    int start = 0, end = 7;
    char * dataStringWithParity;
    for (int i = 0; i < strlen(deFramedString)/8; i++){
        dataStringWithParity = sliceStringC(deFramedString, start, end);
        start += 8, end += 8;
        if (strcmp(parityCheck(dataStringWithParity), "odd parity") == 0){
            char * a = sliceStringC(dataStringWithParity, 1, 7);
            strcat(strcat(dataBits, "0"), a);
        }
        //else printf("There's an error in the data bit!!!");
    }
    return dataBits;
}

char * crcErrorDetection(char * receivedFrame) {
    char * remainder = malloc(sizeof(char) * 33); //32 for reminder crc32 + 1 for null
    unsigned long crc = crc32(0, (const unsigned char *) receivedFrame, strlen(receivedFrame));
    char *crc2 = DecimalToBinary2(crc);
    strcpy(remainder, crc2);
    return remainder;
}

char * creatingError(int n, char * noError){
    //flips the nth bit in each frame
    int length = (int) strlen(noError);
    char errorBits[10000];
    strcpy(errorBits, noError);
    char * returnThis = malloc(sizeof(char) * length + 1);
    if (noError[n] == '1'){
        errorBits[n] = '0';
    }
    else errorBits[n] = '1';
    strcpy(returnThis, errorBits);
    return returnThis;
}

char * creatingError2(int n1, int n2, char * noError){
    //flips two bits in positions n1 and n2 in each frame
    int length = (int) strlen(noError);
    char errorBits[10000];
    strcpy(errorBits, noError);
    char * returnThis = malloc(sizeof(char) * length + 1);
    if (noError[n1] == '1'){
        errorBits[n1] = '0';
    }
    else errorBits[n1] = '1';

    if (noError[n2] == '1'){
        errorBits[n2] = '0';
    }
    else errorBits[n2] = '1';
    strcpy(returnThis, errorBits);
    return returnThis;
}

char* hammingErrorDetectionAndCorrection(char *receivedMessageBits){
    char * returnThis = (char *)malloc(sizeof(char) * 523);
    char bitsWithoutParity[522]= {};
    int error = 0;
    int positionIndex;

    //calculate the number of parity bits added. 2^p >= p + p + 1;
    int noOfParityBitsAdded = 0;
    int lengthOfReceivedMessage = (int)strlen(receivedMessageBits);
    for (int i = 1; i <= lengthOfReceivedMessage; i++) {
        if (log2(i) == (int) log2(i)) {
            noOfParityBitsAdded++;
        }
    }

    //
    char messageBits[522]; int l = 0;
    while(*receivedMessageBits){
        messageBits[l++] = *receivedMessageBits++;
    }
    messageBits[l] = '\0';

    // checking whether there are any errors. use position instead of position index;
    for(int i = 0; i < noOfParityBitsAdded; i++) {
        int count = 0;
        int position = (int)pow(2,i);
        positionIndex = position - 1;
        while(positionIndex < lengthOfReceivedMessage){
            for(int j = positionIndex; j < positionIndex + position; j++){
                if (messageBits[j] == '1') count++;
            }
            positionIndex += 2 * position;
        }
        if(count % 2 != 0){
            error += position;
        }
    }
    (error > 0 && error < 512) ? printf("Error Detected in position: %d\n", error-1) : printf("No error detected\n");

    // navigating to the errornous bits & correct them
    if (error > 0 && error < 512) {
        printf("Error corrected by flipping the bit\n");
        if (messageBits[error - 1] == '1') {
            messageBits[error - 1] = '0';
        }
        else {
            messageBits[error - 1] = '1';
        }

        //return the corrected bits
        int m = 0;
        for (int i = 1; i <= lengthOfReceivedMessage; i++){
            if (log2(i) != (int)log2(i)){
                bitsWithoutParity[m++] = messageBits[i-1];
            }
        }
        strcpy(returnThis, bitsWithoutParity);
        return returnThis;
    }

    //if no error occurred, remove the hamming parity bits and return the original frame transmitted;
    else {
        int p = 0;
        for (int i = 1; i <= lengthOfReceivedMessage; i++) {
            if (log2(i) != (int) log2(i)) {
                bitsWithoutParity[p++] = messageBits[i - 1];
            }
        }
        strcpy(returnThis, bitsWithoutParity);
        return returnThis;
    }
}