# Error-Detection-And-Correction-Module
The aim of this project is to use both Hamming and CRC algorithms to detect errors in transmitted messages between the consumer and producer by means of CRC and to both detect and correct transmission errors by using Hamming.
##### keywords
Error detection, Error correction, hamming code, crc.

## Description
The producer takes the input from the user file which are ASCII characters, converts
each character into binary, encodes each character (8 bits binary) with a parity bit,
and writes the output to a pipe. Each frame contains 2 syn characters, a control
character that contains the length of the data to be transmitted, and the data
transmitted. CRC is applied to the whole frame, whereas hamming is applied to the
data bits.

The consumer on the other end reads the binary bits from the pipe, gets each frame
transmitted using the syn bits, removes the frame, checks if there is error in the
codeword received and obtains the data bits. The data bits are checked for parity to
ensure that there was no error during transmission, the parity bit is dropped, and the
bits are converted back to ASCII converting all characters to upper case. The ASCII
is then saved in another file or printed on the screen. In the absence of error, the
original user data and the data from the consumer would be the same except that the
consumer output is in the upper case. CRC detects error in the frames, where as
hamming detects errors and corrects 1 bit error.

## CRC VS HAMMING
Cyclic Redundancy Check (CRC): CRC is a technique for detecting errors in
a transmitted message.
Consumer side: Given the message bits (m), and the generator bits(g), g – 1 zero’s
is appended to the message bits, to generate the temporary codeword. The temporary
codeword is then divided by the generator bit using XOR operation. The remainder
after this operation is then appended to the message bit to get the final codeword.
The codeword is then transmitted to the receiver.At the Receiver: The codeword is divided by the generator bits, and XOR operation
applied. If the remainder after this operation is zero, then there was no error in
transmission, but if the remainder is not zero, the there is an error in the received
message.

Hamming Code: is a technique for detecting errors in a transmitted message
and correcting the error by flipping the erroneous bit. The Hamming code can
detect errors but can correct if it is a single-bit error.

At the producer: The producer side hamming code involves two steps: Adding the
parity bits and calculating the parity bits. First, the character ‘p’ is added to the
message bits at every position corresponding to the power of 2. For instance, p is
added to positions 1, 2, 4, 8, … up to the length of the message, using the inequality
2 p = m + p + 1. After adding the ‘p’ in the right position, the value of p is calculated
depending on whether even or odd parity is used. Assuming even parity, p 1 is
calculated by counting all the 1’s in the positions 3, 5, 7, 9, … up to the length of the
message, p 2 is calculated by counting all the 1’s in the positions 3, 6 – 7, 10 – 11, …
This is done for all the ‘p’. For each ‘p’, if the number of 1’s counted is even then p
in that position is replaced by ‘0’, otherwise it is replaced by ‘1’.

At the consumer: The parity bits are calculated at the consumer using the same
technique described above. If the parity bits are correct there was no error during the
transmission and the parity bits are dropped to recover the original message, if there
is an error in the transmitted message, some of the parity bits will be wrong. The
wrong parity bits are added, the sum gives the position of the erroneous message bit.
The bit is flipped to recover the message.
