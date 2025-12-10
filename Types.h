#ifndef TYPES_H
#define TYPES_H

// Rule 209: The basic types of int, short, long, float and double shall not be used,
// but specific-length equivalents should be typedef'd.
typedef double float64;
typedef float  float32;
typedef int    int32;
typedef bool   boolean; // JSF often prefers explicit boolean type or just bool if allowed, but Rule 209 focuses on numeric.
                        // Rule 214 says "The bool type will be used for boolean values". So bool is fine.

#endif // TYPES_H