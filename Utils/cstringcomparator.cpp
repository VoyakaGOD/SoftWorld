#include "cstringcomparator.h"

bool CStringComparator::operator()(char const *left, char const *right) const
{
    return strcmp(left, right) < 0;
}
