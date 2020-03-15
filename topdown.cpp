#include "SFML/Graphics.hpp"
#include "SFML/Window.hpp"
#include "SFML/System.hpp"
#include <iostream>
#include <vector>
#include <cmath>
#include <ctime>
#include "noise.h"

int main(){
    float gridSizeF = 10.0f; // Gridsize of the map
    unsigned gridSizeU = static_cast<unsigned>(gridSizeF);
    float viewSpeed = 500.0f; // speed of view
    float dt = 0.f; // For changing of the time
    sf::Clock dtClock;
    sf::RenderWindow window(sf::VideoMode(1000, 1000), "Terrain", sf::Style::Close);
    sf::View view;

    sf::RectangleShape player(sf::Vector2f(gridSizeF*2.0f,gridSizeF*2.0f));
    player.setFillColor(sf::Color::Blue);

    // Size of terrain
    const int height = 100;
    const int width = 100;

    std::vector<std::vector<sf::RectangleShape> > tileMap;
    tileMap.resize(width, std::vector<sf::RectangleShape>());

    // Initializes the grids
    for(int x = 0; x < width; x++){
        tileMap[x].resize(width,sf::RectangleShape());
        for(int y = 0; y < height; y++){
            tileMap[x][y].setSize(sf::Vector2f(gridSizeF,gridSizeF));
            tileMap[x][y].setPosition(x * gridSizeF, y * gridSizeF);
        }
    }
    // Color grids
    int octave2D = 2; // 1 - 8 I think
    double zoom = 5.0f, point = 3.0f, scale = 2.0f;

    for(int x = 0; x < width; x++){
        for(int y = 0; y < height; y++){
            int color = (int)(noise2d(x,y,octave2D,zoom,point,scale));
            if(color > 120)
                tileMap[x][y].setFillColor(sf::Color::Black);
            else
                tileMap[x][y].setFillColor(sf::Color::White);
        }
    }
    
    while (window.isOpen()){
        dt = dtClock.restart().asSeconds();

        sf::Event ev;

        while(window.pollEvent(ev)){
            switch(ev.type){ 
                case sf::Event::Closed:
                    window.close();
                    break;
                default:
                    break;
            }
            // Move the view
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::A))
                player.move(-viewSpeed*dt, 0.f);
            else if(sf::Keyboard::isKeyPressed(sf::Keyboard::D))
                player.move(viewSpeed*dt, 0.f);
            else if(sf::Keyboard::isKeyPressed(sf::Keyboard::W))
                player.move(0.f,-viewSpeed*dt);
            else if(sf::Keyboard::isKeyPressed(sf::Keyboard::S))
                player.move(0.f,viewSpeed*dt);
        }

        window.clear();
        window.setView(view);

        // Draw tilemap
        for(int x = 0; x < width; x++){
            for(int y = 0; y < height; y++)
                window.draw(tileMap[x][y]);
        }

        window.draw(player);

        window.setView(window.getDefaultView());
        window.display();
    }
}
//g++ topdown.cpp -o t -lsfml-graphics -lsfml-window -lsfml-system