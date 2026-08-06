/**
 * Handy preprocessor macros and utility functions for use with embedded programs
 * Author(s): Reece Kibble, Ronald Tun
 */

#include "cmt_utils.h"
#include "string.h"

void utils_reverse(uint8_t *str, uint16_t len)
{
    for (uint16_t i = 0; i < len / 2; i++)
    {
        SWAP(*(str + i), *(str + (len - 1) - i));
    }
}

uint8_t *utils_itoa(int64_t num, uint8_t *str)
{
    uint8_t i = 0;
    int64_t sign;

    //
    if ((sign = num) < 0)
    {
        num = -num;
    }

    do
    {
        str[i++] = num % 10 + '0';
    } while ((num /= 10) > 0);

    // If number is negative, append '-'
    if (sign < 0)
    {
        str[i++] = '-';
    }

    str[i] = '\0'; // Append string terminator

    // Reverse the string
    utils_reverse(str, i);

    return str;
}

int64_t utils_atoi(uint8_t *str)
{
    int64_t result = 0;
    uint8_t digit;
    uint8_t sign = 0;
    uint8_t i = 0;

    // Getting the sign
    if (*str == '-')
    {
        sign = 1;
        i = 1;
    }
    else
    {
        if (*str == '+')
        {
            i = 1;
        }
    }

    // Converting to digits
    for (; str[i] != '\0'; i++)
    {
        if (str[i] < '9' && str[i] >= '0')
        {
            digit = str[i] - '0';
        }
        else
        {
            digit = str[i];
        }
        result = (10 * result) + digit;
    }

    if (sign)
    {
        return -result;
    }
    return result;
}

/*****************************************
 * @brief Convert a float to byte values
 * @param value input float value
 * @param bytes passthrough array, will get populated with values
 *****/
void floatToBytes(float value, uint8_t *bytes)
{
    memcpy(bytes, &value, sizeof(float));
}

/*****************************************
 * @brief Convert a byte array to float
 * @param bytes input arrray
 *****/
float bytesToFloat(const uint8_t *bytes)
{
    float value;
    memcpy(&value, bytes, sizeof(float));
    return value;
}
