#ifndef eagle_EagleUtils_h
#define eagle_EagleUtils_h

#include <mach/mach_time.h>
#include "EagleBoolean.h"

/**
 Minimum value.
 */
#define MIN(X, Y) ((X) < (Y) ? (X) : (Y))

/**
 Safe string concatination.
 */
#define strcat_safe(dest, src) { if(NULL != (dest) && NULL != (src)) { strncat(dest, src, strlen(src)); } }

/**
 Compare two strings without case-sensitivity.
 If \p s1 or \p s2 is NULL then the result will be EagleFalse unless both string are NULL - in which case the result
 will be EagleTrue.
 @param [in] s1 First string
 @param [in] s2 Second string.
 @return EagleTrue is the two strings are the same length and contain the same characters (of any case)
 */
EagleBoolean EagleUtils_CompareWithoutCase(const char *s1, const char *s2);

/**
 Convert a string to lower case. This will modify the string parsed in effectivly destorying the original case.
 @param [in,out] str The string.
 */
void EagleUtils_ToLowerCase(char *str);

/**
 Create a new lowercase copy of a string.
 @param [in] str The string.
 @return A new allocated lowercase version of \p str.
 */
char* EagleUtils_ToLowerCaseCopy(const char *str);

/**
 * Get the absolute time. One second is 1 billion clock ticks.
 * @param [in] mock If this is not zero then this value will be returned. You must set this to 0 to return the true
 * absolute time.
 * @return The absolute time of the mocked time.
 */
uint64_t EagleUtils_GetAbsoluteTime(void);

extern uint64_t EagleUtils_MockAbsoluteTime;

#endif
