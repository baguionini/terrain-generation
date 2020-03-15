#include "SFML/Graphics.hpp"
#include "SFML/Window.hpp"
#include "SFML/System.hpp"
#include <iostream>
#include <vector>
#include <cmath>
#include <ctime>
#include "noise.h"

int main(){
    float gridSizeF = 5.0f; // Gridsize of the map
    unsigned gridSizeU = static_cast<unsigned>(gridSizeF);
    float viewSpeed = 200.0f; // speed of view
    float dt = 0.f; // For changing of the time
    sf::Clock dtClock;
    sf::RenderWindow window(sf::VideoMode(1920, 1080), "Terrain", sf::Style::Close);
    sf::View view;

    view.setSize(1920.0f,1080.0f);
    view.setCenter(window.getSize().x/2.f,window.getSize().y/2.f);

    // Size of terrain
    const int height = 500;
    const int width = 500;
    double scale = 10.0f;

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
        for(int x = 0; x < width; x++){
        for(int y = 0; y < height; y++){
            
            int color = (int)(noise2d(x,y/scale,2,5,3, scale));
            if(color > 120)
                tileMap[x][y].setFillColor(sf::Color::Blue);
            else
                tileMap[x][y].setFillColor(sf::Color::Green);
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
                view.move(-viewSpeed*dt, 0.f);
            else if(sf::Keyboard::isKeyPressed(sf::Keyboard::D))
                view.move(viewSpeed*dt, 0.f);
            else if(sf::Keyboard::isKeyPressed(sf::Keyboard::W))
                view.move(0.f,-viewSpeed*dt);
            else if(sf::Keyboard::isKeyPressed(sf::Keyboard::S))
                view.move(0.f,viewSpeed*dt);
        }

        window.clear();
        window.setView(view);

        // Draw tilemap
        for(int x = 0; x < width; x++){
            for(int y = 0; y < height; y++)
                window.draw(tileMap[x][y]);
        }

        window.setView(window.getDefaultView());
        window.display();
    }
}
//g++ topdown.cpp -o t -lsfml-graphics -lsfml-window -lsfml-system