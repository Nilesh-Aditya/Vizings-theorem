#pragma once
#ifdef _WIN32
#include <SDL.h>
#include <SDL_ttf.h>

#else
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#endif

#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <cmath>

class Screen
{
public:
    const Uint32 SCREEN_HEIGHT = 633;
    const Uint32 SCREEN_WIDTH = 633;
    Screen() : gScreenSurface(nullptr), gCurrentSurface(nullptr), m_window(nullptr), m_renderer(nullptr), m_texture(nullptr), gFont(nullptr) {}
    void initialisation(int n, int edge, std::vector<std::vector<int>> &v);
    bool init();
    void getPoints(std::vector<std::vector<int>> &v, int n);
    void drawLine(std::vector<std::pair<float, float>> &points, std::vector<std::vector<int>> &v);
    void generateColors(Uint8 &red, Uint8 &green, Uint8 &blue);
    void update();
    void close();
    void clear();
    void eventManager();
    void degree(int v, int e, std::vector<std::vector<int>> &edg);
    void colorEdge(std::vector<std::vector<int>> &edg, int e);
    bool quit = false;
    void rendering_text(std::string text, int size, int x, int y, int red = 0xFF, int green = 0xFF, int blue = 0xFF);

private:
    SDL_Surface *gScreenSurface;
    SDL_Surface *gCurrentSurface;
    SDL_Window *m_window;
    SDL_Renderer *m_renderer;
    SDL_Texture *m_texture;
    TTF_Font *gFont;
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
            m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED);
            SDL_SetRenderDrawColor(m_renderer, 0x00, 0x00, 0x00, 0xFF);
        }
        TTF_Init();
    }

    return success;
}

void Screen::getPoints(std::vector<std::vector<int>> &v, int n)
{
    int edges = v.size();
    int k = n;
    int i = 0;
    std::vector<std::pair<float, float>> points(k);
    int x = 100;
    float r = 200;

    while (k)
    {
        float angle = (i * 2 * M_PI) / n;
        points[i].first = SCREEN_WIDTH / 2 + r * std::cos(angle);
        points[i].second = SCREEN_HEIGHT / 2 + r * std::sin(angle);
        // std::cout << points[i].first << "," << points[i].second << std::endl;
        i++;
        k--;
    }

    drawLine(points, v);
};

void Screen::generateColors(Uint8 &red, Uint8 &green, Uint8 &blue)
{
    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_int_distribution<int> random(0, 255);
    int r = random(rng);
    int g = random(rng);
    int b = random(rng);
    // red = (red == r) ? red + 30 : r;
    // green = (green == g) ? green + 40 : g;
    // blue = (blue == b) ? blue + 50 : b;
    red = r;
    blue = b;
    green = g;
}

void Screen::drawLine(std::vector<std::pair<float, float>> &points, std::vector<std::vector<int>> &v)
{
    clear();
    Uint8 red = 0;
    Uint8 green = 0;
    Uint8 blue = 0;
    int vertice = points.size();
    int edge = v.size();
    std::vector<bool> check(edge, true);

    for (size_t i = 0; i < v.size(); i++)
    {
        generateColors(red, green, blue);
        // std::cout << red << "," << green << "," << blue << std::endl;
        SDL_SetRenderDrawColor(m_renderer, red, green, blue, SDL_ALPHA_OPAQUE);
        if (check[i])
        {
            SDL_RenderDrawLine(m_renderer, points[v[i][0] - 1].first, points[v[i][0] - 1].second, points[v[i][1] - 1].first, points[v[i][1] - 1].second);
            int x_mid = (points[v[i][0] - 1].first + points[v[i][1] - 1].first) / 2;
            int y_mid = (points[v[i][0] - 1].second + points[v[i][1] - 1].second) / 2;
            rendering_text("c" + std::to_string(v[i][2]), 28, x_mid, y_mid);
            check[i] = false;
            int temp = v[i][2];
            for (size_t j = 0; j < v.size(); j++)
            {
                if (j != i && v[j][2] == temp)
                {
                    SDL_RenderDrawLine(m_renderer, points[v[j][0] - 1].first, points[v[j][0] - 1].second, points[v[j][1] - 1].first, points[v[j][1] - 1].second);
                    int mid_x = (points[v[j][0] - 1].first + points[v[j][1] - 1].first) / 2;
                    int mid_y = (points[v[j][0] - 1].second + points[v[j][1] - 1].second) / 2;
                    rendering_text("c" + std::to_string(v[i][2]), 28, mid_x, mid_y);
                    check[j] = false;
                }
            }
        }
    }
    SDL_RenderPresent(m_renderer);
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
    SDL_SetRenderDrawColor(m_renderer, 0x00, 0x00, 0x00, 0xFF);
    SDL_RenderClear(m_renderer);
}

void Screen::close()
{
    SDL_DestroyTexture(m_texture);
    m_texture = nullptr;
    TTF_CloseFont(gFont);
    SDL_DestroyRenderer(m_renderer);
    SDL_DestroyWindow(m_window);
    m_window = nullptr;
    m_renderer = nullptr;
    SDL_Quit();
}

void Screen::rendering_text(std::string text, int size, int x, int y, int red /* = 0xFF*/, int green /*= 0xFF*/, int blue /*= 0xFF*/)
{
    gFont = TTF_OpenFont("oswald/Oswald-Medium.ttf", size);
    SDL_Color textColor = {red, green, blue};

    SDL_Surface *textSurface = TTF_RenderText_Solid(gFont, text.c_str(), textColor);
    m_texture = SDL_CreateTextureFromSurface(m_renderer, textSurface);
    int width = textSurface->w;
    int height = textSurface->h;
    SDL_Rect textRect = {x - width / 2, y - height / 2, width, height};
    SDL_RenderCopy(m_renderer, m_texture, 0, &textRect);
    SDL_FreeSurface(textSurface);
}