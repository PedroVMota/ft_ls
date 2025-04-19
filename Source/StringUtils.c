#include "../Header/StringUtils.h"

size_t string_len(char *str)
{
    if (str == NULL) {
        return 0;
    }
    size_t len = 0;
    while (str[len] != '\0') {
        len++;
    }
    return len;
}

size_t dptr_len(void **dptr, size_t ptr_size)
{
    size_t i = 0;
    if (dptr == NULL)
        return 0;
    while(dptr[i] != NULL) {
        i++;
    }
    return i;
}


char *duplicate_string(char *str)
{
    if (str == NULL) {
        return NULL;
    }
    size_t len = string_len(str);
    char *new_str = malloc((len + 1) * sizeof(char));
    if (new_str == NULL) {
        perror("Failed to allocate memory");
        return NULL;
    }
    for (size_t i = 0; i < len; i++) {
        new_str[i] = str[i];
    }
    new_str[len] = '\0';  // Null-terminate the new string
    return new_str;
}


char **append_child(char **array, char *newelement){
    size_t len = dptr_len((void **)array, sizeof(char *));
    char **new_array = malloc((len + 2) * sizeof(char *));
    if (new_array == NULL) {
        perror("Failed to allocate memory");
        return NULL;
    }
    for (size_t i = 0; i < len; i++) {
        new_array[i] = array[i];
    }

    new_array[len] = duplicate_string(newelement);  // <-- isso é essencial!
    if (new_array[len] == NULL) {
        perror("Failed to allocate memory for element");
        free(new_array);
        return NULL;
    }

    new_array[len + 1] = NULL;
    free(array);
    return new_array;
}



char *concat_strings(size_t count, ...)
{
    /* First pass – compute total length */
    size_t total_len = 0;

    va_list ap;
    va_start(ap, count);
    for (size_t i = 0; i < count; ++i) {
        const char *s = va_arg(ap, const char *);
        if (s)
            total_len += strlen(s);
    }
    va_end(ap);

    /* +1 for terminating NUL */
    char *result = malloc(total_len + 1);
    if (!result)
        return NULL;

    /* Second pass – copy strings into the buffer */
    result[0] = '\0';          /* start with empty string */

    va_start(ap, count);
    for (size_t i = 0; i < count; ++i) {
        const char *s = va_arg(ap, const char *);
        if (s)
            strcat(result, s); /* safe: we pre‑sized the buffer */
    }
    va_end(ap);

    return result;
}
