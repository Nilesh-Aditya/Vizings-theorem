#include "screen.hpp"

int main()
{
    Screen screen;

    if (screen.init())
    {
        screen.initialisation();
    }

    return 0;
}