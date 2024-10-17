#define MSTRING_IMPL
#include "../mstring.h"

double mstring_into_double(mstring str)
{
    return DEREF(double, mstring_convert(str, "%lf", sizeof(double)));
}

int main()
{
    mstring before_point = mstring_new("232");
    mstring numeric = mstring_join_cstr(before_point, ".2131");
    
    printf("%lf\n", mstring_into_double(numeric));

    mstring_free(before_point);
    mstring_free(numeric);

    return 0;
}
