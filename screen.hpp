#pragma once
#include "SDL2/SDL.h"
#include <iostream>
#include <vector>
#include <unordered_map>
#include <time.h>

class Screen
{
public:
    Uint32 SCREEN_HEIGHT = 633;
    Uint32 SCREEN_WIDTH = 480;
    Screen() : gScreenSurface(NULL), gCurrentSurface(NULL), m_window(NULL), gRenderer(NULL) {}
    void initialisation();
    bool init();
    void getPoints(std::vector<std::vector<int>> &v, int n);
    void drawLine(std::unordered_map<int, int> &mp, std::vector<std::vector<int>> &v);
    void generateColors(Uint8 &red, Uint8 &green, Uint8 &blue);
    void update();
    void close();
    void clear();
    void degree(int v, int e, std::vector<std::vector<int>> &edg);
    void colorEdge(std::vector<std::vector<int>> &edg, int e);

private:
    SDL_Surface *gScreenSurface;
    SDL_Surface *gCurrentSurface;
    SDL_Window *m_window;
    SDL_Renderer *gRenderer;
    std::vector<std::vector<int>> v;
};

void Screen::initialisation()
{
    srand(time(NULL));
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

    getPoints(edg, v);
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
            gRenderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED);
            SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
        }
    }

    return success;
}

void Screen::getPoints(std::vector<std::vector<int>> &v, int n)
{
    int edges = v.size();
    int k = n;
    std::unordered_map<int, int> mp;
    while (k)
    {
        Uint32 RAND_X = (rand() / RAND_MAX) * SCREEN_WIDTH;
        Uint32 RAND_Y = (rand() / RAND_MAX) * SCREEN_HEIGHT;
        auto it = mp.find(RAND_X);
        if (it != mp.end())
        {
            mp[RAND_X] = RAND_Y;
            k--;
        }
        else
            continue;
    }

    drawLine(mp, v);
};

void Screen::generateColors(Uint8 &red, Uint8 &green, Uint8 &blue)
{
    int elapsed = SDL_GetTicks();
    red = (1 + sin(elapsed * 0.0001)) * 128;
    green = (1 + sin(elapsed * 0.0002)) * 128;
    blue = (1 + sin(elapsed * 0.0003)) * 128;
}

void Screen::drawLine(std::unordered_map<int, int> &mp, std::vector<std::vector<int>> &v)
{
    clear();
    Uint8 red;
    Uint8 green;
    Uint8 blue;
}

void Screen::clear()
{
    SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(gRenderer);
}

void Screen::close()
{
    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(m_window);
    m_window = NULL;
    gRenderer = NULL;
    SDL_Quit();
}