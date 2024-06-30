#include "SpanSet.h"
#include <iostream>

int main()
{
    SpanSet<int> spanSet;
    std::cout << spanSet.Add({ 0,60 }) << '\n';
    std::cout << spanSet.Remove({ -10, 10 }) << '\n';
    std::cout << spanSet.Remove({ 50, 1000 }) << '\n';
    std::cout << spanSet.Remove({ 30, 40 }) << '\n';
    std::cout << spanSet.Remove({ 30, 40 }) << '\n';
    std::cout << spanSet.Remove({ 50, 60 }) << '\n';


    std::cout << "\n\n\n";

    SpanSet<float> fspanSet;
    std::cout << fspanSet.Add({ 0, 1 }) << "\n";
    std::cout << fspanSet.Remove({ 0.2f, 0.5f }) << "\n";
    std::cout << fspanSet.Remove({ 0.7f, 0.8f }) << "\n";

    return 0;
}
