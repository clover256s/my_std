#include <vector.h>

#include <array>
#include <iostream>
#include <vector>

void tests() {
    my_std::vector< int > vec = { 60000000, 500000, 40000, 3000, 200, 10 };

    vec.inspect();
    vec.shrink_to_fit();
}

int main() {
    tests();
    return 0;
}
