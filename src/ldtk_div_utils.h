// Copied from https://stackoverflow.com/questions/828092/python-style-integer-division-modulus-in-c

#pragma once

namespace ldtk
{

[[nodiscard]] constexpr int py_mod(int a, int b)
{
    return ((a % b) + b) % b;
}

[[nodiscard]] constexpr int py_div(int a, int b)
{
    return (a - (py_mod(a, b))) / b;
}

} // namespace ldtk
