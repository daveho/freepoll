#ifndef DATATYPES_H
#define DATATYPES_H

#include <cstdint>

// remote ID
typedef uint32_t RemoteID;

// multiple choice option
enum class Option {
  A, B, C, D, E,
};

// timestamp: milliseconds since the Unix epoch
typedef uint64_t Timestamp;

#endif // DATATYPES_H
