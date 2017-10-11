#include "encryption.h"

std::string encrypt(std::string text)
{
    char key = 'x';
    std::string encrypted;
    std::string original = text;

    for (int i = 0; i < original.size(); i++)
    {
        encrypted += original[i] ^ (int(key) + i) % 20;
    }

    return encrypted;
}

std::string decrypt(std::string text)
{
    char key = 'x';
    std::string decrypted;
    std::string encrypted = text;

    for (int i = 0; i < encrypted.size(); i++)
    {
        decrypted += encrypted[i] ^ (int(key) + i) % 20;
    }

    return decrypted;
}
