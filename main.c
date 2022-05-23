#include <ctype.h>
#include <math.h>
#include "zlib.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include "MainProducer.h"
#include "MainConsumer.h"

int main() {
    // Read the user file and store it in the string array. Then convert to Binary
    char inputFilePath[100];
    char outputFilePath[100];
    printf("Please enter input file directory: ");
    scanf("%s", inputFilePath);
    printf("Enter binary output file directory: ");
    scanf("%s", outputFilePath);

    // Reads a binary file from the producer and store in string variable binaryData
    char textOutputFilePath[100];
    printf("Enter ASCII output file directory: ");
    scanf("%s", textOutputFilePath);

    //Select An Error Detection/ Correction Technique
    int errorDetectionTechnique; int bitToFlip; int bitToFlip2;
    printf("Choose Error Detection and correction technique\n1. CRC\n2. Hamming Code\n"
           "3. CRC with error\n4. Hamming with single-bit error\n5. Hamming with 2 bits Error per frame\n");
    scanf("%d", &errorDetectionTechnique);

    if (errorDetectionTechnique == 3 || errorDetectionTechnique == 4){
        printf("Enter bit position to flip: \n");
        scanf("%d", &bitToFlip);
    }

    if (errorDetectionTechnique == 5){
        printf("Enter first bit position to flip: \n");
        scanf("%d", &bitToFlip);
        printf("Enter second bit position to flip: \n");
        scanf("%d", &bitToFlip2);
    }

    //Pipe
    int fd[2];
    if (pipe(fd) < 0){
        printf("pipe failed");
        exit(1);
    }

    //Fork to create Parent and Child process
    int id = fork();
    if (id < 0){
        printf("Fork failed");
        return 1;
    }

    //Child process
    else if (id == 0){
        close(fd[0]);
        char *binaryBits = mainProducer(inputFilePath, outputFilePath, errorDetectionTechnique);
        write(fd[1], binaryBits, (int)strlen(binaryBits));
        //close(fd[1]);
    }

    //Parent Process: it waits for the child process to execute first
    else{
        close(fd[1]);
        char bitsFromPipe[9000];
        read(fd[0], bitsFromPipe, 9000);
        mainConsumer(bitsFromPipe, textOutputFilePath, errorDetectionTechnique, bitToFlip, bitToFlip2);
        wait(NULL);
        close(fd[0]);
    }
    return 0;
}
