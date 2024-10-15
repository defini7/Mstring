#define MSTRING_IMPL
#include "../mstring.h"

int main()
{
    mstring hello = mstring_new("Hello, ");
    mstring world = mstring_new("world!");

    mstring hello_world = mstring_join(hello, world);

    printf("%s\n", mstring_raw(hello_world));

    mstring_free(hello);
    mstring_free(world);
    mstring_free(hello_world);

    return 0;
}
