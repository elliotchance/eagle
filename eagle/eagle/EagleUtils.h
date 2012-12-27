#ifndef eagle_EagleUtils_h
#define eagle_EagleUtils_h

/**
 Minimum value.
 */
#define MIN(X, Y) ((X) < (Y) ? (X) : (Y))

/**
 Safe string concatination.
 */
#define strcat_safe(dest, src) { if(NULL != (dest) && NULL != (src)) { strncat(dest, src, strlen(src)); } }

#endif
