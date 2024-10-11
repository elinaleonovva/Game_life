#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <time.h>
#include <conio.h>
#include <windows.h>
#include <iostream>
#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 640
#define BORDER_SIZE 1
#define CELL_SIZE_NO_BORDER 30 //������ ������ � �������� (��������) 10 20
#define CELL_SIZE CELL_SIZE_NO_BORDER - 2 * BORDER_SIZE //������ ���������� ����� ������ 
#define MAP_WIDTH 60
#define MAP_HEIGHT 30
#define FPS 12 //�������� ������������

using namespace std;
using namespace sf;

bool Generation[MAP_WIDTH][MAP_HEIGHT] = {false }; //����� ���������
bool OldGeneration[MAP_WIDTH][MAP_HEIGHT] = { false }; //����������� ����������� ���������


void Generate() //������������� ������ ��������� � ����� ����������� ���
{
    for (int x = 0; x < MAP_WIDTH; x++) {
        for (int y= 0; y < MAP_HEIGHT; y++) {
            if (rand() % 10 == 1) {
                Generation[x][y] = OldGeneration[x][y] = true;
            }
            else {
                Generation[x][y] = OldGeneration[x][y] = false;
            }
        }
    }
}

void UpdateOldGeneration() //���������� ����������� ��������� 
{
    for (int x = 0; x < MAP_WIDTH; x++)
    {
        for (int y = 0; y < MAP_HEIGHT; y++)
        {
            OldGeneration[x][y] = Generation[x][y];
        }
    }
}

char GetCell(char x, char y) // ����������� ����
{
    int newX = x, newY = y;
    if ((x < 0) || (x > MAP_WIDTH - 1) || (y < 0) || (y > MAP_HEIGHT - 1))
    {
        if (x < 0) {
            newX = MAP_WIDTH - 1;
        }
        else if (x > MAP_WIDTH - 1) {
            newX = 0;
        }

        if (y < 0) {
            newY = MAP_HEIGHT - 1;
        }
        else if (y > MAP_HEIGHT - 1) {
            newY = 0;
        }
    }
    return (char)OldGeneration[newX][newY];
}

char GetCellsCount(char x, char y) // ���������� ����� ������ ����� ������ �� ������� ����������� ���������
{
    return GetCell(x - 1, y) + GetCell(x - 1, y - 1) + GetCell(x, y - 1) + GetCell(x + 1, y - 1) +
        GetCell(x + 1, y) + GetCell(x + 1, y + 1) + GetCell(x, y + 1) + GetCell(x - 1, y + 1);
}

void Update()
{
    UpdateOldGeneration(); // ���������� ���������� ���������
    for (int x = 0; x < MAP_WIDTH; x++)
    {
        for (int y = 0; y < MAP_HEIGHT; y++)
        {
            char cellsCount = GetCellsCount(x, y); // ���������� ���������� ������� ������ 
            if (OldGeneration[x][y] == false) // ������ ���� ������� 
            {
                if (cellsCount == 3) // ���-�� ������� ����� 3 �� ������ �������
                {
                    Generation[x][y] = true;
                }
            }
            else
            {
                if ((cellsCount < 2) || (cellsCount > 3)) // �������� � ��������� �������
                {
                    Generation[x][y] = false;
                }
            }
        }
    }
}

void updateCellOnMouse(int x, int y) { // ������, ������������� �����
    int cellX, cellY;
    cellX = (x - BORDER_SIZE * 2) / CELL_SIZE_NO_BORDER;
    cellY = (y - BORDER_SIZE * 2) / CELL_SIZE_NO_BORDER;
    Generation[cellX][cellY] = !Generation[cellX][cellY];
}

void Draw(RenderWindow* window, RectangleShape* aliveCell, RectangleShape* deadCell) // ��������� ������ ������
{
    for (int x = 0; x < MAP_WIDTH; x++)
    {
        for (int y = 0; y < MAP_HEIGHT; y++)
        {
            if (Generation[x][y]) // ���� ������ �����, �� ������������ �� 
            {
                aliveCell->setPosition(x * CELL_SIZE + 4 * BORDER_SIZE, y * CELL_SIZE + 4 * BORDER_SIZE);
                window->draw(*aliveCell);
            }
            else {
                deadCell->setPosition(x * CELL_SIZE + 4 * BORDER_SIZE, y * CELL_SIZE + 4 * BORDER_SIZE);
                window->draw(*deadCell);
            }
        }
    }
}

int main()
{
    srand(time(NULL));
    RenderWindow window(VideoMode(CELL_SIZE_NO_BORDER * MAP_WIDTH + 2 * BORDER_SIZE, CELL_SIZE_NO_BORDER * MAP_HEIGHT + 2 * BORDER_SIZE), "Life Game"); // �������� ����
    window.setFramerateLimit(500); // �������� ��������� ��������
    RectangleShape aliveCell, deadCell; // ������� ����� � �����������
    aliveCell.setFillColor(Color(250, 50, 50)); // ��������� �����
    aliveCell.setSize(Vector2f(CELL_SIZE, CELL_SIZE)); // ��� ��� �������� �������� ������ 
    aliveCell.setOutlineColor(Color::Black);
    aliveCell.setOutlineThickness(BORDER_SIZE);
    deadCell.setFillColor(Color(50, 50, 50)); // ��������� �����
    deadCell.setSize(Vector2f(CELL_SIZE, CELL_SIZE)); // ��� ��� �������� �������� ������ 
    deadCell.setOutlineColor(Color::Black);
    deadCell.setOutlineThickness(2.);
    int fps = FPS;

    bool paused = true;

    while (window.isOpen())
    {
        Event event; // ������� 
        while (window.pollEvent(event)) // ���� ���� �������, ������������ ��
        {
            if (event.type == Event::Closed) // �������� �� ��� ������� 
                window.close();

            else if (event.type == sf::Event::KeyPressed)
            {
                if (Keyboard::isKeyPressed(Keyboard::R))
                {
                    if (paused) {
                        Generate();
                    }
                }
                else if (Keyboard::isKeyPressed(Keyboard::Escape))
                {
                    return 0;
                }

                else if (Keyboard::isKeyPressed(Keyboard::Space))
                {
                    paused = !paused;
                    if (paused) {
                        window.setFramerateLimit(500);
                    }
                    else {
                        window.setFramerateLimit(fps); 
                    }
                }

                else if (Keyboard::isKeyPressed(Keyboard::Up))
                {
                    window.setFramerateLimit(++fps); 
                }

                else if (Keyboard::isKeyPressed(Keyboard::Down))
                {
                    window.setFramerateLimit(--fps);
                }
            }
            else if (Mouse::isButtonPressed(Mouse::Left)) {
                if (paused) {
                    updateCellOnMouse(Mouse::getPosition(window).x, Mouse::getPosition(window).y);
                }
            }
        }

        if (!paused) {
            Update();
        }
        window.clear(); // �������� ����
        Draw(&window, &aliveCell, &deadCell); // ��������� ������ ��������� 
        window.display(); // ����������� ��������
    }
    return 0;
}