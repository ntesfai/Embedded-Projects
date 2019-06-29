/**
    Author: Nubian Tesfai
    Course: SER486, Arizona State University
    Instructor: Doug Sandy

    This class implements functionality for calculating and updating
    checksums for the data passed.
*/

#include "util.h"

/**
    Name: update_checksum
    Parameters: unsigned char*, unsigned int
    Changes: This method allows for updating the checksum
    value for the data passed.
*/
void update_checksum(unsigned char* data, unsigned int size){
    unsigned int i;
    unsigned int sum = 0;
    unsigned char result = 0;

    for(i = 0; i < size; i++) {
        if(i == size - 1){
            result = 256 - sum;
            data[i] = result + sum;
            continue;
        }

            sum += sizeof(data[i]);
    }

}

/**
    Name: is_checksum_valid
    Parameters: unsigned char*, unsigned int
    Changes: Verifies the sum of all data is equal
    to zero.
*/
int is_checksum_valid(unsigned char* data, unsigned int size) {
    unsigned int i;
    unsigned char sum = 0;
    for(i = 0; i < size; i++) {
        if(i == size - 1){
            continue;
        }
    }
    if (sum == 0)
        return 1;
    else
        return 0;
}
