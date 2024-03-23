#ifndef CSTRINGCOMPARATOR_H
#define CSTRINGCOMPARATOR_H

#include <cstring>

using namespace std;

class CStringComparator
{
public:
    bool operator()(char const *left, char const *right) const;
};

#endif // CSTRINGCOMPARATOR_H
