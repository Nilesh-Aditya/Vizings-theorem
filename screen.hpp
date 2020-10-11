#pragma once
#include "SDL2/SDL.h"
#include <iostream>
#include <vector>

class Screen
{
public:
    const static unsigned SCREEN_HEIGHT = 633;
    const static unsigned SCREEN_WIDTH = 1063;
    Screen() : gScreenSurface(NULL), gCurrentSurface(NULL), m_window(NULL) {}
    void initialisation();
    bool init();
    bool loadmedia();
    void update();
    void close();
    void degree(int v, int e, std::vector<std::vector<int>> &edg);
    void colorEdge(std::vector<std::vector<int>> &edg, int e);

private:
    SDL_Surface *gScreenSurface;
    SDL_Surface *gCurrentSurface;
    SDL_Window *m_window;
    std::vector<std::vector<int>> v;
};

void Screen::initialisation()
{
    std::cout << "Enter Number of vertices : " << std::endl;
    int n;
    std::cin >> n;
    // std::vector<std::vector<int>> v;
    std::cout << "Enter number of edges : " << std::endl;
    int edge;
    std::cin >> edge;

    int j = edge;
    int i = 1;
    int ctr = 1;

    while (j)
    {
        std::vector<int> temp;
        i = i % n;
        if (i % n == 0)
        {
            i = n;
            // ctr++;
        }
        temp.push_back(i % edge);
        if ((i + ctr) % n != 0)
            temp.push_back((i + ctr) % n);
        else
            temp.push_back(i + ctr);

        temp.push_back(-1);

        if (i == n)
            ctr++;
        v.push_back(temp);
        i++;
        j--;
    }
    degree(n, edge, v);
}

void Screen::degree(int v, int e, std::vector<std::vector<int>> &edg)
{
    int maximum = -1;
    int Deg[v + 1] = {};

    for (int i = 0; i < e; i++)
    {
        Deg[edg[i][0]]++;
        Deg[edg[i][1]]++;
    }

    // To find the maximum degree.
    for (int i = 1; i <= v; i++)
    {
        if (maximum < Deg[i])
            maximum = Deg[i];
    }

    // color the edges of the graph.
    colorEdge(edg, e);
    std::cout << maximum + 1 << " colors needs to generate"
              << " a valid edge coloring :\n";

    for (int i = 0; i < e; i++)
        std::cout << "\ncolor between v(1): " << edg[i][0] << " and v(2): "
                  << edg[i][1] << " is: color " << edg[i][2];
    std::cout << std::endl;
}

void Screen::colorEdge(std::vector<std::vector<int>> &edg, int e)
{
    int color;

    // Assign a color to every edge 'i'.
    for (int i = 0; i < e; i++)
    {
        color = 1;
    flag:
        // Assign a color and
        // then check its validity.
        edg[i][2] = color;
        for (int j = 0; j < e; j++)
        {
            if (j == i)
                continue;

            // If the color of edges
            // is adjacent to edge i
            if ((edg[i][0] == edg[j][0]) ||
                (edg[i][1] == edg[j][0]) ||
                (edg[i][0] == edg[j][1]) ||
                (edg[i][1] == edg[j][1]))
            {

                // If the color matches
                if (edg[i][2] == edg[j][2])
                {

                    // Increment the color,
                    // denotes the change in color.
                    color++;

                    // goes back, and assigns
                    // the next color.
                    goto flag;
                }
            }
        }
    }
}

bool Screen::init()
{
    bool success = true;
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
        success = false;
    }
    else
    {
        //Create window
        m_window = SDL_CreateWindow("VIZING'S THEOREM (DISCRETE PROJECT)", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if (m_window == NULL)
        {
            printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
            success = false;
        }
        else
        {
            //Initialize PNG loading
            gScreenSurface = SDL_GetWindowSurface(m_window);
        }
    }

    return success;
}
