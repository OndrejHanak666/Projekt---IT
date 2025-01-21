#include "Minesweeper.h"
#include <iostream>
#include <time.h>
#include <SFML/Graphics.hpp>

using namespace sf;

void initializeGrid(int grid[12][12], int sgrid[12][12])
{
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            sgrid[i][j] = 10;
            if (rand() % 5 == 0)
                grid[i][j] = 9; // Mine
            else
                grid[i][j] = 0; // Empty
        }
    }

    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            if (grid[i][j] == 9) continue; // Skip mines
            int n = 0;
            if (grid[i + 1][j] == 9) n++;
            if (grid[i][j + 1] == 9) n++;
            if (grid[i - 1][j] == 9) n++;
            if (grid[i][j - 1] == 9) n++;
            if (grid[i + 1][j + 1] == 9) n++;
            if (grid[i - 1][j - 1] == 9) n++;
            if (grid[i - 1][j + 1] == 9) n++;
            if (grid[i + 1][j - 1] == 9) n++;
            grid[i][j] = n;
        }
    }
}

bool checkWin(int grid[12][12], int sgrid[12][12])
{
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            if (grid[i][j] != 9 && sgrid[i][j] == 10)
                return false; // Je�t� jsou neodhalen� pol��ka
        }
    }
    return true;
}

void revealEmpty(int x, int y, int grid[12][12], int sgrid[12][12])
{
    if (x < 0 || x >= 10 || y < 0 || y >= 10) return; // Mimo hranice
    if (sgrid[x][y] != 10) return; // U� odhaleno nebo ozna�eno

    sgrid[x][y] = grid[x][y]; // Odhalit aktu�ln� pol��ko

    if (grid[x][y] != 0) return; // P�estat, pokud je ��slo

    // Rekurzivn� odhalit sousedn� pol��ka
    revealEmpty(x - 1, y, grid, sgrid);
    revealEmpty(x + 1, y, grid, sgrid);
    revealEmpty(x, y - 1, grid, sgrid);
    revealEmpty(x, y + 1, grid, sgrid);
    revealEmpty(x - 1, y - 1, grid, sgrid);
    revealEmpty(x + 1, y + 1, grid, sgrid);
    revealEmpty(x - 1, y + 1, grid, sgrid);
    revealEmpty(x + 1, y - 1, grid, sgrid);
}

void playMinesweeper()
{
    srand(time(0));

    RenderWindow game(VideoMode(400, 400), "MINESWEEPER!!");

    int w = 32;
    int grid[12][12];
    int sgrid[12][12];

    Texture t1;
    t1.loadFromFile("image/tiles2.jpg");
    Sprite s(t1);

    Font font;
    font.loadFromFile("font/arial.ttf");

    Text timerText("", font, 20);
    timerText.setFillColor(Color::Black);
    timerText.setPosition(10, 330);

    Text statusText("", font, 30);
    statusText.setFillColor(Color::Black);
    statusText.setPosition(10, 360);

    Clock clock;
    bool gameOver = false;
    bool gameWon = false;

    initializeGrid(grid, sgrid);


    while (game.isOpen())
    {
        Vector2i pos = Mouse::getPosition(game);
        int x = pos.x / w;
        int y = pos.y / w;


        Event e;
        while (game.pollEvent(e))
        {
            if (e.type == Event::Closed)
                game.close();

            if (e.type == Event::KeyPressed)
            {
                if (e.key.code == Keyboard::Escape)
                {
                    // U�ivatelsk� n�vrat do hlavn� nab�dky
                    return; // Opust� funkci a vr�t� se do hlavn� nab�dky
                }
            }

            if (!gameOver && !gameWon && e.type == Event::MouseButtonPressed)
            {
                if (e.key.code == Mouse::Left)
                    if (grid[x][y] == 9)
                    {
                        gameOver = true;
                        for (int i = 0; i < 10; i++)
                            for (int j = 0; j < 10; j++)
                                if (sgrid[i][j] == 10)
                                    sgrid[i][j] = grid[i][j];
                    }
                    else if (grid[x][y] == 0)
                    {
                        revealEmpty(x, y, grid, sgrid);
                        gameWon = checkWin(grid, sgrid);
                    }
                    else
                    {
                        sgrid[x][y] = grid[x][y];
                        gameWon = checkWin(grid, sgrid);
                    }
                else if (e.key.code == Mouse::Right)
                    sgrid[x][y] = 11;
            }


            if (e.type == Event::KeyPressed)
                if (e.key.code == Keyboard::R)
                {
                    initializeGrid(grid, sgrid);
                    gameOver = false;
                    gameWon = false;
                    clock.restart();
                }
        }

        if (!gameOver && !gameWon)
        {
            int elapsed = clock.getElapsedTime().asSeconds();
            timerText.setString("Cas: " + std::to_string(elapsed) + "s");
        }

        // Nastaven� textu stavu hry
        if (gameOver)
        {
            statusText.setString("Prohral jsi!");
        }
        else if (gameWon)
        {
            statusText.setString("Vyhral jsi!");
        }
        else
        {
            statusText.setString("");
        }

        game.clear(Color::White);
        for (int i = 0; i < 10; i++)
            for (int j = 0; j < 10; j++)
            {
                if (sgrid[x][y] == 9) sgrid[i][j] = grid[i][j];
                s.setTextureRect(IntRect(sgrid[i][j] * w, 0, w, w));
                s.setPosition(i * w, j * w);
                game.draw(s);

            }
        game.draw(timerText);
        game.draw(statusText);
        game.display();
    }
}

