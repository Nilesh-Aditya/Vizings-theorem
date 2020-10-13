#pragma once
#ifdef _WIN32
#include "SDL.h"
#else
#include "SDL2/SDL.h"
#endif

#include <iostream>
#include <vector>
#include <algorithm>
#include <time.h>
#include <random>

class Screen
{
public:
    Uint32 SCREEN_HEIGHT = 633;
    Uint32 SCREEN_WIDTH = 480;
    Screen() : gScreenSurface(nullptr), gCurrentSurface(nullptr), m_window(nullptr), gRenderer(nullptr) {}
    void initialisation(int n, int edge, std::vector<std::vector<int>> &v);
    bool init();
    void getPoints(std::vector<std::vector<int>> &v, int n);
    void drawLine(std::vector<std::pair<int, int>> &points, std::vector<std::vector<int>> &v);
    void generateColors(Uint8 &red, Uint8 &green, Uint8 &blue);
    void update();
    void close();
    void clear();
    void eventManager();
    void degree(int v, int e, std::vector<std::vector<int>> &edg);
    void colorEdge(std::vector<std::vector<int>> &edg, int e);
    bool quit = false;

private:
    SDL_Surface *gScreenSurface;
    SDL_Surface *gCurrentSurface;
    SDL_Window *m_window;
    SDL_Renderer *gRenderer;
};

void Screen::initialisation(int n, int edge, std::vector<std::vector<int>> &v)
{
    // srand(time(nullptr));
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

    // getPoints(edg, v);
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
        if (m_window == nullptr)
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
    int i = 0;
    std::vector<std::pair<int, int>> points(k);
    std::random_device rd;
    std::mt19937 rng(rd());
    while (k)
    {
        std::uniform_int_distribution<int> RAND_X(5, SCREEN_WIDTH - 10);
        std::uniform_int_distribution<int> RAND_Y(5, SCREEN_HEIGHT - 10);
        // Uint32 RAND_Y = (rand() / RAND_MAX) * SCREEN_HEIGHT;
        int x = RAND_X(rng);
        int y = RAND_Y(rng);
        auto p = std::make_pair(x, y);
        auto it = std::find_if(points.begin(), points.end(), [&p](const std::pair<int, int> &element) { return element.first == p.first; });
        if (it == points.end())
        {
            points[i].first = x;
            points[i].second = y;
            i++;
            k--;
        }
        else
            continue;
    }

    drawLine(points, v);
};

void Screen::generateColors(Uint8 &red, Uint8 &green, Uint8 &blue)
{
    int elapsed = SDL_GetTicks();
    red = (1 + sin(elapsed * 0.0001)) * 128;
    green = (1 + sin(elapsed * 0.0002)) * 128;
    blue = (1 + sin(elapsed * 0.0003)) * 128;
}

void Screen::drawLine(std::vector<std::pair<int, int>> &points, std::vector<std::vector<int>> &v)
{
    clear();
    Uint8 red;
    Uint8 green;
    Uint8 blue;
    int vertice = points.size();
    int edge = v.size();

    for (size_t i = 0; i < points.size() - 1; i++)
    {
        generateColors(red, green, blue);
        SDL_RenderDrawLine(gRenderer, points[i].first, points[i].second, points[i + 1].first, points[i + 1].second);
    }
}

void Screen::eventManager()
{
    SDL_Event e;

    while (SDL_PollEvent(&e) != 0)
    {
        //User requests quit
        if (e.type == SDL_QUIT)
        {
            Screen::quit = true;
        }
    }
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
    m_window = nullptr;
    gRenderer = nullptr;
    SDL_Quit();
}