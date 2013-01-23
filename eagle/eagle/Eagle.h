#ifndef eagle_Eagle_h
#define eagle_Eagle_h

/**
 This struct instance variable's memory is fully managed internally. You should not allocate or free this variable.
 */
#define EAGLE_ATTR_MANAGED

/**
 This struct instance variable may be freed internally or externally.
 */
#define EAGLE_ATTR_SEMI_MANAGED

/**
 This struct instance variable is provided by an external source. It will never be allocated or freed internally.
 */
#define EAGLE_ATTR_PROVIDED

/**
 This struct instance variable is not dynamically allocated so it's memory does not need to be managed internally or
 externally.
 */
#define EAGLE_ATTR_NA

#endif
