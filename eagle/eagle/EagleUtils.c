#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "EagleUtils.h"

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
