// Copied from https://stackoverflow.com/a/38999752/12875525

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
