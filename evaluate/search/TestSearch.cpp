#include <string>
#include <cstring>
#include <algorithm>
#include <iostream>
#include <sstream>

using namespace std;

int main()
{

    const char* SearchBuffer = "This is a small Test how to find a string position in a c-Buffer";

    string SearchFor("Test");

    char* SearchBegin = const_cast<char*>(SearchBuffer);
    char* SearchEnd = const_cast<char*>(SearchBuffer+strlen(SearchBuffer));

    char* Found = search(SearchBegin, SearchEnd, SearchFor.begin(), SearchFor.end());

    cout << "Found:" << Found << " Size:" << (Found-SearchBegin) << endl;

}
