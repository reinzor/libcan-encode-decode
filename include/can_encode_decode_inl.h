#ifndef CAN_ENCODE_DECODE_INL_H_
#define CAN_ENCODE_DECODE_INL_H_

#include <stdint.h> //uint typedefinitions, non-rtw!

#define MASK64(nbits) ((0xffffffffffffffff)>> (64-nbits))

inline float toPhysicalValue(uint64_t target, float factor, float offset, bool is_signed)
{
    if (is_signed)
        return ( (int64_t) target ) * factor + offset;
    else
        return target * factor + offset;
}

inline uint64_t fromPhysicalValue(float physical_value, float factor, float offset)
{
    return (physical_value - offset) / factor;
}

void storeSignal(uint8_t* frame, uint64_t value, const uint8_t startbit, const uint8_t length, bool is_big_endian, bool is_signed)
{
    uint8_t start_byte = startbit / 8;
    uint8_t startbit_in_byte = startbit % 8;
    uint8_t end_byte = 0;
    int8_t count = 0;
    uint8_t current_target_length = (8-startbit_in_byte);

    //! TODO: Deal with sign
    if (is_signed)
    {
        // perform sign extension
        // update value
    }

    value &= MASK64(length);

    frame[start_byte] |= value >> startbit_in_byte;
    if(is_big_endian) // Motorola (big endian)
    {
        end_byte = (start_byte * 8 + 8 - startbit_in_byte - length) / 8;

        for(count = start_byte-1; count >= end_byte; count --)
        {
            frame[count] |= value << current_target_length;
            current_target_length += 8;
        }
    }
    else // Intel (little endian)
    {
        end_byte = (startbit + length - 1) / 8;
        for(count = start_byte+1; count <= end_byte; count ++)
        {
            frame[count] |= value << current_target_length;
            current_target_length += 8;
        }
    }
}

inline uint64_t extractSignal(const uint8_t* frame, const uint8_t startbit, const uint8_t length, bool is_big_endian, bool is_signed)
{
    uint8_t start_byte = startbit / 8;
    uint8_t startbit_in_byte = startbit % 8;
    uint8_t end_byte = 0;
    int8_t count = 0;
    uint64_t target = frame[start_byte] >> startbit_in_byte;
    uint8_t current_target_length = (8-startbit_in_byte);

    if(is_big_endian) // Motorola (big endian)
    {
        end_byte = (start_byte * 8 + 8 - startbit_in_byte - length) / 8;

        for(count = start_byte-1; count >= end_byte; count --)
        {
            target |= frame[count] << current_target_length;
            current_target_length += 8;
        }
    }
    else // Intel (little endian)
    {
        end_byte = (startbit + length) / 8;

        for(count = start_byte+1; count <= end_byte; count ++)
        {
            target |= frame[count] << current_target_length;
            current_target_length += 8;
        }
    }    

    target &= MASK64(length);

    if (is_signed)
    {
        // perform sign extension
        int64_t msb_sign_mask = 1 << (length - 1);
        target = ( (int32_t) target ^ msb_sign_mask) - msb_sign_mask;
    }

    return target;
}

inline float decode(const uint8_t* frame, const uint16_t startbit, const uint16_t length, bool is_big_endian, bool is_signed, float factor, float offset)
{
    return toPhysicalValue(extractSignal(frame, startbit, length, is_big_endian, is_signed), factor, offset, is_signed);
}

#endif
