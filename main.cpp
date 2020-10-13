#include "screen.hpp"

int main()
{
    std::vector<std::vector<int>> v;
    std::cout << "Enter Number of vertices : " << std::endl;
    int n;
    std::cin >> n;
    // std::vector<std::vector<int>> v;
    std::cout << "Enter number of edges : " << std::endl;
    int edge;
    std::cin >> edge;
    Screen screen;

    if (screen.init())
    {

        screen.initialisation(n, edge, v);
        screen.getPoints(v, n);

        while (!screen.quit)
        {
            screen.eventManager();
        }
    }
    else
    {
        std::cout << "Unable to load" << std::endl;
    }

    screen.close();

#ifdef _WIN32
    std::cout << "Press enter to exit." << std::endl;
    std::cin.get();
#endif

    return 0;
}