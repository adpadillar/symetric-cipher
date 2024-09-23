#include "../include/confusion.h"
#include <iostream>
#include <string>

void confuse_1(char message[], char key[], char output[])
{
    // Construct an array of length 256 filled with chars (8bits)
    char arr[257] = "hfrteyurito0p87654pkjhgfdlnbgshyjsm34567890hfdrteyurito0p87654pkjhgfdlnbgshyjsm34567890hfdrteyurito0p87654pkjhgfdlnbgshyjsm34567890hfdrteyurito0p87654pkjhgfdlnbgshyjsm34567890hfdrteyurito0p87654pkjhgfdlnbgshyjsm34567890hfdrteyurito0p87654pkjhgfdlnbgshyjsj1";

    // Map each character in the key to a value in the array
    char changed_key[16];
    for (int i = 0; i < 16; i++)
    {
        changed_key[i] = arr[key[i]];
    }

    // XOR the changed key with the message
    char xor_result[16];
    for (int i = 0; i < 16; i++)
    {
        xor_result[i] = changed_key[i] ^ message[i];
    }

    // Obtain the complementary color of the output bytes
    for (int i = 0; i < 16; i++)
    {
        output[i] = static_cast<unsigned char>(255 - static_cast<unsigned char>(xor_result[i]));
    }
}

void confuse_2(char message[], char key[], char output[])
{
    // Rotate the key by the first character of the key
    int n = key[0] % 16;

    char rotated_key[16];
    for (int i = 0; i < 16; i++)
    {
        rotated_key[i] = key[(i + n) % 16];
    }

    // XOR the message with the rotated key
    for (int i = 0; i < 16; i++)
    {
        output[i] = message[i] ^ rotated_key[i];
    }
}

void deconfuse_1(char cypher[], char key[], char output[])
{
    // Construct an array of length 256 filled with chars (8bits)
    char arr[257] = "hfrteyurito0p87654pkjhgfdlnbgshyjsm34567890hfdrteyurito0p87654pkjhgfdlnbgshyjsm34567890hfdrteyurito0p87654pkjhgfdlnbgshyjsm34567890hfdrteyurito0p87654pkjhgfdlnbgshyjsm34567890hfdrteyurito0p87654pkjhgfdlnbgshyjsm34567890hfdrteyurito0p87654pkjhgfdlnbgshyjsj1";

    char changed_key[16];
    for (int i = 0; i < 16; i++)
    {
        changed_key[i] = arr[key[i]];
    }

    // Reverse the complementary color from the XOR result
    char xor_result[16];
    for (int i = 0; i < 16; i++)
    {
        xor_result[i] = static_cast<unsigned char>(255 - static_cast<unsigned char>(cypher[i]));
    }

    // Reverse the XOR operation to obtain the original message
    for (int i = 0; i < 16; i++)
    {
        output[i] = xor_result[i] ^ changed_key[i];
    }
}

void deconfuse_2(char cypher[], char key[], char output[])
{
    // implementation is identical to confuse_2
    confuse_2(cypher, key, output);
}
