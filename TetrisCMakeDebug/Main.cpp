#include <SFML/Graphics.hpp>
#include <time.h>
#include <string>
#include <iostream>
#include "Header.h"

using namespace std;
using namespace sf;

const int M = 22; // ������ �������� ����
const int N = 10; // ������ �������� ����

int field[M][N] = { 0 }; // ������� ����

int ax[4], ay[4], bx[4], by[4];


bool check() // �������� ���������� ������ � ��������� �����
{
    for (int i = 0; i < 4; i++)
        if (ax[i] < 0 || ax[i] >= N || ay[i] >= M) return 0;
        else if (field[ay[i]][ax[i]]) return 0;

    return 1;
};

class rotatetetromino {
    public:
        void rot(int ax[4], int ay[4], int bx[4], int by[4]) {
            int px = ax[1], py = ay[1]; // ����� ��������
            for (int i = 0; i < 4; i++)
            {
                int x = ay[i] - py; int y = ax[i] - px;
                ax[i] = px - x; ay[i] = py + y;
            }
            if (!check())
                for (int i = 0; i < 4; i++) {
                    ax[i] = bx[i]; ay[i] = by[i];
                }
        }
};

int main()
{
    srand(time(0));

    rotatetetromino rotate;

    // �������� ����
    RenderWindow window(VideoMode(320, 480), "The Game!");

    // ��������
    Texture t1, t2, t3;
    t1.loadFromFile("..\\TetrisCMakeDebug\\images\\tiles.png");
    t2.loadFromFile("..\\TetrisCMakeDebug\\images\\background.png");
    t3.loadFromFile("..\\TetrisCMakeDebug\\images\\frame.png");

    Sprite s(t1), background(t2), frame(t3); // �������� ��������

    int dx = 0; // �������������� ��������
    int colorNum = 1; // ����� �����
    float timer = 0; // ������� �������
    float delay = 0.3; // ����� ��������
    bool beginGame = true; // ���� ������
    sf::Text text; sf::Font font;
    font.loadFromFile("..\\TetrisCMakeDebug\\sfns-display-bold.ttf");
    string points = "0";

    Clock clock; // ����

    while (window.isOpen())
    {
        // ����� � ������ � ��������
        float time = clock.getElapsedTime().asSeconds();
        clock.restart();
        timer += time;

        // ��� ������� ���������
        if (beginGame) { beginGame = false; Rand(ax, ay); }

        Event e;
        while (window.pollEvent(e))

        {
            if (e.type == Event::Closed) // ������������ ����� �� �������
                window.close();

            if (e.type == Event::KeyPressed) // ������� ������ �� ����������
                if (e.key.code == Keyboard::Up) rotate.rot(ax, ay, bx, by); //�������
                else if (e.key.code == Keyboard::Left) dx = -1; // �����
                else if (e.key.code == Keyboard::Right) dx = 1; // ������
        }

        if (Keyboard::isKeyPressed(Keyboard::Down)) delay = 0.05; // �������� �������

        //// <- �������� -> ///
        for (int i = 0; i < 4; i++)
        { 
            bx[i] = ax[i]; // ���������� ����������� ���������
            by[i] = ay[i];
            ax[i] += dx; // �������
        }
        if (!check()) // ���� ������� �� ����������� 
            for (int i = 0; i < 4; i++) {
                ax[i] = bx[i]; // ���������� �������
                ay[i] = by[i];
            }       

        ///////�������//////
        if (timer > delay)
        {
            for (int i = 0; i < 4; i++) //������� ����
            { bx[i] = ax[i]; by[i] = ay[i]; ay[i] += 1; }

            if (!check())
            {
                for (int i = 0; i < 4; i++) // ����������� ������
                    field[by[i]][bx[i]] = colorNum;

                colorNum = 1 + rand() % 7; // ������ ���� ��������
                Rand(ax, ay);
            }

            timer = 0; // �������� �������
        }

        ///�������� �� ��������
        for (int i = 0; i < N; i++) {
            if (field[1][i] != 0) {
                window.close();
            }
        }

        ///////����������� �����//////////
        int k = M - 1; // ��������� ������ ������
        for (int i = M - 1; i > 0; i--) // ����� ����� ��������� ������
        {
            int count = 0; // ������� ����������� ���������
            for (int j = 0; j < N; j++)
            {
                if (field[i][j]) count++; // �������� �� �������� 
                field[k][j] = field[i][j];
            }
            if (count < N) {
                k--;
            }
            if (count == 10) {
                points = to_string(stoi(points) + 1);
            }
        }

        // ���������� �����
        text.setFont(font); text.setString("Points: " + points);
        text.setCharacterSize(20); text.setPosition(12, 0);
        text.setColor(sf::Color::Color(255, 215, 0));
        text.setStyle(sf::Text::Bold | sf::Text::Underlined);

        dx = 0; delay = 0.3; // ���������� ��������

        /////////����//////////
        window.clear(Color::White);
        window.draw(background); // �� ��� ��������

        for (int i = 0; i < M; i++)
            for (int j = 0; j < N; j++)
            {
                if (field[i][j] == 0) continue;
                s.setTextureRect(IntRect(field[i][j] * 18, 0, 18, 18)); // �������� ������
                s.setPosition(j * 18, i * 18); // ������� ������ ������
                s.move(28, -5); // ������� ����
                window.draw(s); // ���������
            }

        for (int i = 0; i < 4; i++) // ��������� ���������
        {
            s.setTextureRect(IntRect(colorNum * 18, 0, 18, 18));
            s.setPosition(ax[i] * 18, ay[i] * 18);
            s.move(28, -5); 
            window.draw(s);
        }

        window.draw(frame);
        window.draw(text);
        window.display();
    }

    return 0;
}
