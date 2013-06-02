#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <mach/mach_time.h>
#include "EagleUtils.h"
#include "EagleMemory.h"

uint64_t EagleUtils_MockAbsoluteTime = 0;

uint64_t EagleUtils_GetAbsoluteTime(void)
{
    /* return the mocked time */
    if(0 != EagleUtils_MockAbsoluteTime) {
        return EagleUtils_MockAbsoluteTime;
    }
    
    /* return the true absolute time */
    return mach_absolute_time();
}

char* EagleUtils_ToLowerCaseCopy(const char *str)
{
    unsigned long len;
    char *r;
    
    if(NULL == str) {
        return NULL;
    }
    
    len = strlen(str) + 1;
    r = (char*) EagleMemory_Allocate("EagleUtils_ToLowerCaseCopy.1", len);
    if(NULL == r) {
        return NULL;
    }
    
    memmove(r, str, len);
    EagleUtils_ToLowerCase(r);
    return r;
}

void EagleUtils_ToLowerCase(char *str)
{
    unsigned long i;
    
    for(i = 0; i < strlen(str); ++i) {
        str[i] = (char) tolower(str[i]);
    }
}

EagleBoolean EagleUtils_CompareWithoutCase(const char *s1, const char *s2)
{
    size_t s1len, i;
    
    /* if either of the strings is NULL then don't both comparing them, however if they are both NULL then we must
     return EagleTrue */
    if(NULL == s1 || NULL == s2) {
        return (NULL == s1 && NULL == s2);
    }
    
    /* the strings are different lengths, so also don't bother comparing them */
    s1len = strlen(s1);
    if(strlen(s2) != s1len) {
        return EagleFalse;
    }
    
    /* check each character */
    for(i = 0; i < s1len; ++i) {
        if(tolower(s1[i]) != tolower(s2[i])) {
            return EagleFalse;
        }
    }
    
    /* the strings must be equal */
    return EagleTrue;
}
