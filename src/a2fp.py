import struct

def float_to_applesoft_fp(value: float, verbose: bool = False) -> bytearray:
    """Convert an IEEE float (Python) to the Applesoft 5 byte floating point format

    :param value: The IEEE float to convert
    :type value: float
    :param verbose: If True, include debugging output. Defaults to False
    :type verbose: bool, optional
    :return: The Applesoft 5 byte floating point (memory) format
    :rtype: bytearray
    """
    
    packed = struct.pack('f', value)   # convert to 32 bit float
    float_int = struct.unpack('I', packed)[0]    # cast to 32bit unsigned int
    
    # unpack the ieee 754 bits
    sign = (float_int >> 31) & 0x1               # Shift past 31 bits, mask 1 bit (0=pos, 1=neg)
    exponent = ((float_int >> 23) & 0xFF) - 127  # Shift past 23 bits, mask 8 bits (0xFF), bias 127)
    mantissa = float_int & 0x7FFFFF              # Mask the lowest 23 bits (0x7FFFFF)

    if verbose:
        print(f"IEEE 754 sign: {sign}, exponent: {exponent}, mantissa: {mantissa:023b}")
    
    # Convert to Applesoft format
    v = bytearray(5)
    # First byte is the exponent
    v[0] = exponent + 129
    # Next 4 are the mantissa and the sign bit
    i = (mantissa << 8) | (sign << 31)
    # Store it in the bytes
    v[1] = (i >> 24) & 0xFF
    v[2] = (i >> 16) & 0xFF
    v[3] = (i >> 8) & 0xFF
    v[4] = (i >> 0) & 0xFF

    return v

def applesoft_fp_to_float(value: bytearray, verbose: bool = False) -> float:
    """Convert a float in Applesoft 5 byte format into an IEEE float

    :param value: The five byte Applesoft format float representation
    :type value: bytearray
    :param verbose: If True include debugging output. Defaults to False
    :type verbose: bool, optional
    :return: The IEEE float
    :rtype: float
    """
    # Extract the bits from the Applesoft 5 byte format
    exponent = value[0] - 129        # Offset 128 exponent
    sign = (value[1] & 0x80) >> 7    # Sign is the highest bit of the first byte
    mantissa = value[1] & 0x7F       # 31 bit mantissa
    mantissa = (mantissa << 8) | value[2]
    mantissa = (mantissa << 8) | value[3]
    mantissa = (mantissa << 8) | value[4] 

    if verbose:
        print(f"Applesoft sign: {sign}, exponent: {exponent}, mantissa: {mantissa:031b}")

    # build an IEEE 754 float from the fields extracted from the AS FAC FP
    v = (sign << 31)               # sign bit
    v |= ((exponent + 127) << 23)   # exponent
    v |= (mantissa >> 8)            # drop the extra 8bits of the mantissa
            
    return struct.unpack('f', struct.pack('I', v))[0]


def double_to_applesoft_fp(value: float, verbose: bool = False) -> bytearray:
    """Convert an IEEE double (Python) to the Applesoft 5 byte floating point format

    :param value: The IEEE double to convert
    :type value: float
    :param verbose: If True, include debugging output. Defaults to False
    :type verbose: bool, optional
    :return: The Applesoft 5 byte floating point (memory) format
    :rtype: bytearray
    """
    
    packed = struct.pack('d', value)   # convert to 64 bit float
    double_int = struct.unpack('Q', packed)[0]    # cast to 64bit unsigned int
    
    # unpack the ieee 754 bits
    sign = (double_int >> 63) & 0x1                # Shift past 63 bits, mask 1 bit (0=pos, 1=neg)
    exponent = ((double_int >> 52) & 0x7FF) - 1023 # Shift past 52 bits, mask 8 bits (0x7FF), bias 1023)
    mantissa = double_int & 0xFFFFFFFFFFFFF        # Mask the lowest 52 bits (0xFFFFFFFFFFFFF)

    if verbose:
        print(f"IEEE 754 sign: {sign}, exponent: {exponent}, mantissa: {mantissa:052b}")
    
    # Convert to Applesoft format
    v = bytearray(5)
    # First byte is the exponent
    v[0] = exponent + 129
    # Next 4 are the mantissa and the sign bit
    i = (mantissa >> 21) | (sign << 31)
    # Store it in the bytes
    v[1] = (i >> 24) & 0xFF
    v[2] = (i >> 16) & 0xFF
    v[3] = (i >> 8) & 0xFF
    v[4] = (i >> 0) & 0xFF

    return v

def applesoft_fp_to_double(value: bytearray, verbose: bool = False) -> float:
    """Convert a float in Applesoft 5 byte format into an IEEE double

    :param value: The five byte Applesoft format float representation
    :type value: bytearray
    :param verbose: If True include debugging output. Defaults to False
    :type verbose: bool, optional
    :return: The IEEE double
    :rtype: float
    """
    # Extract the bits from the Applesoft 5 byte format
    exponent = value[0] - 129        # Offset 128 exponent
    sign = (value[1] & 0x80) >> 7    # Sign is the highest bit of the first byte
    mantissa = value[1] & 0x7F       # 31 bit mantissa
    mantissa = (mantissa << 8) | value[2]
    mantissa = (mantissa << 8) | value[3]
    mantissa = (mantissa << 8) | value[4] 

    if verbose:
        print(f"Applesoft sign: {sign}, exponent: {exponent}, mantissa: {mantissa:031b}")

    # build an IEEE 754 float from the fields extracted from the AS FAC FP
    v = (sign << 63)                # sign bit
    v |= ((exponent + 1023) << 52)  # exponent
    v |= (mantissa << 21)           # the 31 bits move to the upper 52 bits
            
    return struct.unpack('d', struct.pack('Q', v))[0]

# Applesoft format
# EEEEEEEE SMMMMMMM MMMMMMMM MMMMMMMM MMMMMMMM

# IEEE 754 float
# SEEEEEEE EMMMMMMM MMMMMMMM MMMMMMMM

# IEEE 754 double
# SEEEEEEE EEEEMMMM MMMMMMMM MMMMMMMM MMMMMMMM MMMMMMMM MMMMMMMM MMMMMMMM
