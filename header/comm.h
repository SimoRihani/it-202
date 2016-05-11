#ifndef _COMM_H_
#define _COMM_H_

/**
 * Parses a string, extracts its tags, allocates the right amout of memory to store them.
 * @Param : const char* - String to parse
 * @Patam : char*** - Unallocated string array. Will contain the tags
 */
void comm_getTags(const char*, char***);

/**
 * Frees the memory allocated by comm_freeTags.
 * @Param : char** - String array to deallocate
 */
void comm_freeTags(char**);

/**
 * Determines if the provided string represents more than one tags or not.
 * @Param : const char* - String to parse
 */
int comm_isSingleTag(const char *);

#endif
