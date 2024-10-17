#ifndef MSTRING_H
#define MSTRING_H

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <limits.h>

#define MSTRING_IS_TRIM(c) (c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\b' || c == '\f')
#define DEREF(type, value) *(type*)(value)

typedef struct __mstring
{
    char* _data;
    size_t _length;
}
_mstring, *mstring;

mstring mstring_alloc();
mstring mstring_new(const char* data);
mstring mstring_copy(mstring str);

void mstring_free(mstring str);

const size_t MSTRING_NPOS = (size_t)(-1);

const char* mstring_raw(mstring str);
size_t mstring_length(mstring str);

bool mstring_empty(mstring str);
bool mstring_contains(mstring str, const char* what);

mstring mstring_substr(mstring str, size_t start, size_t length);
char mstring_at(mstring str, size_t i);

void mstring_clear(mstring str);
void mstring_erase(mstring str, size_t start, size_t end);

size_t mstring_first_index(mstring str, char c);
size_t mstring_last_index(mstring str, char c);

bool mstring_ends_with(mstring str, const char* end);
bool mstring_starts_with(mstring str, const char* end);

mstring mstring_upper(mstring str);
mstring mstring_lower(mstring str);

int mstring_compare(mstring str1, mstring str2);
bool mstring_equals(mstring str1, mstring str2);

mstring mstring_replace(mstring str, const char* what, const char* to);
mstring mstring_remove(mstring str, const char* what);

mstring mstring_join(mstring str1, mstring str2);
mstring mstring_join_cstr(mstring str1, const char* str2);

mstring mstring_trim_left(mstring str);
mstring mstring_trim_right(mstring str);
mstring mstring_trim(mstring str);

void* mstring_convert(mstring str, const char* pattern, size_t type_size);

#ifdef MSTRING_IMPL
#undef MSTRING_IMPL

mstring mstring_alloc()
{
    return (mstring)malloc(sizeof(_mstring));
}

mstring mstring_new(const char* data)
{
    mstring str = mstring_alloc();

    if (data)
    {
        str->_length = strlen(data);
        str->_data = (char*)malloc(str->_length + 1);

        memcpy(str->_data, data, str->_length + 1);
    }
    else
    {
        str->_length = 0;
        str->_data = NULL;
    }

    return str;
}

mstring mstring_copy(mstring str)
{
    if (str && str->_data)
        return mstring_new((const char*)str->_data);

    return NULL;
}

void mstring_free(mstring str)
{
    if (str)
    {
        if (str->_data)
            free(str->_data);

        free(str);
        str = NULL;
    }
}

const char* mstring_raw(mstring str)
{
    if (str)
        return str->_data;

    return NULL;
}

size_t mstring_length(mstring str)
{
    if (str)
        return str->_length;

    return 0;
}

bool mstring_empty(mstring str)
{
    if (str)
        return str->_length == 0;

    return true;
}

bool mstring_contains(mstring str, const char* what)
{
    if (str && str->_data)
        return strstr(str->_data, what) != NULL;

    return false;
}

mstring mstring_substr(mstring str, size_t start, size_t length)
{
    if (!str || !str->_data || length < 0 || start < 0)
        return NULL;

    mstring new_str = (mstring)malloc(sizeof(_mstring));

    new_str->_data = (char*)malloc(length + 1);
    new_str->_length = length;

    strncpy(new_str->_data, str->_data + start, length);
    new_str->_data[new_str->_length] = '\0';

    return new_str;
}

char mstring_at(mstring str, size_t i)
{
    if (str && str->_data)
        return str->_data[i];

    return '\0';
}

void mstring_clear(mstring str)
{
    if (str)
    {
        if (str->_data)
            free(str->_data);

        str->_length = 0;
    }
}

void mstring_erase(mstring str, size_t start, size_t end)
{
    if (!str || !str->_data || start < 0 || start > end)
        return;

    size_t range = end - start + 1;

    strncpy(str->_data + start, str->_data + end + 1, str->_length - end);

    str->_length -= range;
    str->_data = realloc(str->_data, str->_length + 1);
    str->_data[str->_length] = '\0';
}

size_t mstring_first_index(mstring str, char c)
{
    if (str && str->_data)
    {
        for (size_t i = 0; i < str->_length; i++)
        {
            if (str->_data[i] == c)
                return i;
        }
    }

    return MSTRING_NPOS;
}

size_t mstring_last_index(mstring str, char c)
{
    if (str && str->_data)
    {
        for (size_t i = str->_length - 1; i != 0; i--)
        {
            if (str->_data[i] == c)
                return i;
        }
    }

    return MSTRING_NPOS;
}

bool mstring_ends_with(mstring str, const char* end)
{
    if (!str || !str->_data)
        return false;

    size_t end_len = strlen(end);

    for (size_t i = 0; i < end_len; i++)
    {
        if (str->_data[str->_length - i - 1] != end[end_len - i - 1])
            return false;
    }

    return true;
}

bool mstring_starts_with(mstring str, const char* end)
{
    if (!str || !str->_data)
        return false;

    size_t end_len = strlen(end);

    for (size_t i = 0; i < end_len; i++)
    {
        if (str->_data[i] != end[i])
            return false;
    }

    return true;
}

mstring mstring_upper(mstring str)
{
    if (!str || !str->_data)
        return NULL;

    mstring new_str = mstring_copy(str);

    for (size_t i = 0; i < new_str->_length; i++)
        new_str->_data[i] = (char)toupper((char)new_str->_data[i]);

    return new_str;
}

mstring mstring_lower(mstring str)
{
    if (!str || !str->_data)
        return NULL;

    mstring new_str = mstring_copy(str);

    for (size_t i = 0; i < new_str->_length; i++)
        new_str->_data[i] = (char)tolower((char)new_str->_data[i]);

    return new_str;
}

int mstring_compare(mstring str1, mstring str2)
{
    if (str1 && str1->_data && str2 && str2->_data)
        return strcmp((const char*)str1->_data, (const char*)str2->_data);

    return INT_MAX;
}

bool mstring_equals(mstring str1, mstring str2)
{
    return mstring_compare(str1, str2) == 0;
}

mstring mstring_replace(mstring str, const char* what, const char* to)
{
    if (!str || !str->_data || !what || !to)
        return NULL;

    size_t len_what = strlen(what);
    size_t len_to = strlen(to);

    size_t count = 0;

    char* insert_pos = str->_data;
    char* temp = NULL;

    while ((temp = strstr(insert_pos, what)) != NULL)
    {
        insert_pos = temp + len_what;
        count++;
    }

    mstring new_str = mstring_alloc();
    temp = new_str->_data = malloc(str->_length + (len_to - len_what) * count + 1);

    while (count--)
    {
        insert_pos = strstr(str->_data, what);
        size_t len_front = insert_pos - str->_data;
        temp = strncpy(temp, str->_data, len_front) + len_front;
        temp = strcpy(temp, to) + len_to;
        str->_data += len_front + len_what;
    }

    strcpy(temp, str->_data);
    return new_str;
}

mstring mstring_remove(mstring str, const char* what)
{
    if (!str || !str->_data || !what)
        return NULL;

    mstring new_str = mstring_copy(str);

    char *p, *q, *r;

    if (*what && (q = r = strstr(new_str->_data, what)))
    {
        size_t len = strlen(what);

        while (r = strstr(p = r + len, what))
        {
            while (p < r)
                *q++ = *p++;
        }

        while ((*q++ = *p++) != '\0')
            continue;
    }

    return new_str;
}

mstring mstring_join(mstring str1, mstring str2)
{
    if (!str1 || !str1->_data || !str2 || !str2->_data)
        return NULL;

    size_t length = str1->_length + str2->_length;

    char* output = (char*)malloc(length + 1);
    memset(output, 0, length + 1);

    strcat(output, str1->_data);
    strcat(output, str2->_data);

    mstring new_str = mstring_alloc();
    new_str->_data = output;
    new_str->_length = length;

    return new_str;
}

mstring mstring_join_cstr(mstring str1, const char* str2)
{
    if (!str1 || !str1->_data || !str2)
        return NULL;

    size_t length = str1->_length + strlen(str2);

    char* output = (char*)malloc(length + 1);
    memset(output, 0, length + 1);

    strcat(output, str1->_data);
    strcat(output, str2);

    mstring new_str = mstring_alloc();
    new_str->_data = output;
    new_str->_length = length;

    return new_str;
}

mstring mstring_trim_left(mstring str)
{
    if (!str || !str->_data)
        return NULL;

    size_t new_start = 0;

    while (new_start < str->_length && MSTRING_IS_TRIM(str->_data[new_start]))
        new_start++;

    return mstring_substr(str, new_start, str->_length - new_start);
}

mstring mstring_trim_right(mstring str)
{
    if (!str || !str->_data)
        return NULL;

    size_t new_length = str->_length - 1;

    while (new_length > 0 && MSTRING_IS_TRIM(str->_data[new_length]))
        new_length--;

    return mstring_substr(str, 0, new_length + 1);
}

mstring mstring_trim(mstring str)
{
    if (!str || !str->_data)
        return NULL;

    size_t new_start = 0;
    size_t new_end = str->_length - 1;

    while (new_start < str->_length && MSTRING_IS_TRIM(str->_data[new_start]))
        new_start++;

    while (new_end > 0 && MSTRING_IS_TRIM(str->_data[new_end]))
        new_end--;

    return mstring_substr(str, new_start, new_end - new_start + 1);
}

void* mstring_convert(mstring str, const char* pattern, size_t type_size)
{
    if (!str || !str->_data || !pattern)
        return NULL;

    void* value = malloc(type_size);
    sscanf(str->_data, pattern, value);
    return value;
}

#endif

#endif