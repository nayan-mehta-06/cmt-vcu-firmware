/**
 * Handy preprocessor macros and utility functions for use with embedded programs
 * Author(s): Reece Kibble, Ronald Tun
 */

#ifndef _CMT_UTILS_H
    #define _CMT_UTILS_H

    #include <stdint.h>
    #include "stm32f4xx.h"

    typedef enum{
    FALSE,
    TRUE
    }boolean;

    // Shift the value by n bytes
    // eg. BYTESHIFT(0x08, 1), 1000 -> 1000 0000 0000
    #define BYTESHIFT(val, n) ((val) << ((n)*8))

    // Get the specified bit out of multiple bits of data. Results in 1 or 0.
    // eg. GETBIT(0xA1, 3) -> 1010 [0]001 = 0
    #define GETBIT(val, n) (((val) >> (n)) & 0x01)

    /**
     * @brief Set nth bit of input byte to 1
     * @param inByte input number/byte
     * @param n which bit to set
     */
    #define SETBIT(inByte, n) ((inByte) | (1<<n))

    /**
     * @brief Reset nth bit of input byte to 0
     * @param inByte input number/byte
     * @param n which bit to reset
     */
    #define RESETBIT(inByte, n) ((inByte) & ~(1<<n))


    /**
     * @brief Toggle the nth bit
     * @param inByte input number/byte
     * @param n which bit to toggle
     */
    #define TOGGLEBIT(inByte, n) ((inByte) ^ (1<<n))


    /**
     * @brief split 16 bit byte into 2 8 bit bytes
     * @param inVal input 16 bit value
     * @param high first 8 bits
     * @param low last eight bits
     **/
    #define SPLIT_16_TO_8(inVal, high, low) {  \
            (high) = (uint8_t) ((inVal>>8)); \
            (low) = (uint8_t) (inVal);   \
    }

    /**
     * @brief split 32 bit value into 3 8 bit values
     * @param inVal input 32 bit value
     * @param r1 MSByte
     * @param ... middle
     * @param r4 LSByte
     **/
    #define SPLIT_32_TO_8(inVal, r1, r2, r3, r4) {  \
        (r1) = (uint8_t) ((inVal>>24)); \
        (r2) = (uint8_t) ((inVal>>16)); \
        (r3) = (uint8_t) ((inVal>>8)); \
        (r4) = (uint8_t) (inVal); \
    }

    /**
     * @brief append bytes together to make 16 bit number
     * @param high input higher 8 bit
     * @param low input lower 8 bit values
     * @param outputVal output appened value, pass through
     **/
    #define APPEND_BITS2(high, low, outputVal) (outputVal = ((high <<8) | low)) //16 bit


    /**
     * @brief converts an array of 4 bytes into a 32 bit float,
     * !!make sure this array format matches floating point number representation
     * R1 == MSB R4 == LSB
     * @return returns a 32 bit value
     **/
    #define CONVERT_TO_32BIT(r1, r2, r3, r4) \
    ((uint32_t)(r1) << 24) | \
    ((uint32_t)(r2) << 16) | \
    ((uint32_t)(r3) << 8) | \
    (uint32_t)(r4)

	#define CONVERT_TO_SIGNED_32BIT(r1, r2, r3, r4) \
		((int32_t)(r1) << 24) | \
		((int32_t)(r2) << 16) | \
		((int32_t)(r3) << 8) | \
		(int32_t)(r4)

    /**
     * @brief converts an array of 3 bytes into a 24 bit int,
     * !!make sure this array format matches floating point number representation
     * R1 == MSB R4 == LSB
     * @return returns a 32 bit value
     **/
     #define CONVERT_TO_24BIT(r1, r2, r3) \
    ((uint32_t)(r1) << 16) | \
    ((uint32_t)(r2) << 8) | \
    (uint32_t)(r3)

	#define CONVERT_TO_SIGNED_24BIT(r1, r2, r3) \
		((int32_t)(r1) << 16) | \
		((int32_t)(r2) << 8) | \
		(int32_t)(r3)


	#define CONVERT_TO_16BIT(r1, r2) \
	((uint16_t)(r1) << 8) | \
	(uint16_t)(r2)

	#define CONVERT_TO_SIGNED_16BIT(r1, r2) \
		((int16_t)(r1) << 8) | \
		(int16_t)(r2)

    // Get a power of two by shifting left n times.
    // eg. POW2(4) -> 0001 0000 = 16 ( = 2^4 )
    #define POW2(n) (0x01 << (n))

    // Swap the values of a and b.
    #define SWAP(a,b) { \
        (a) ^= (b);     \
        (b) ^= (a);     \
        (a) ^= (b);     \
    }

    //check if byte is even
    #define IS_EVEN(n) (n & 1)


    /**
     * Reverses the given string of length len
     */
    void utils_reverse(uint8_t* str, uint16_t len);

    /**
     * Lightweight implementation of itoa. Converts an integer to a string. A string buffer has to be supplied
     * to the str pointer, and the pointer to this string is also returned.
     */
    uint8_t* utils_itoa(int64_t num, uint8_t* str);

    /**
     * Lightweight implementation of atoi. Converts a string to an integer. Requires pointer to the string
     */
    int64_t utils_atoi(uint8_t* str);

    void floatToBytes(float value, uint8_t* bytes);
    float bytesToFloat(const uint8_t* bytes);


#endif
