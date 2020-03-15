#include "SFML/Graphics.hpp"
#include "SFML/Window.hpp"
#include "SFML/System.hpp"
#include <iostream>
#include <vector>
#include <cmath>
#include <ctime>
#include "noise.h"

int main(){
    srand(time(0));
    float gridSizeF = 20.0f; // Gridsize of the map
    unsigned gridSizeU = static_cast<unsigned>(gridSizeF);
    float viewSpeed = 500.0f; // speed of view duh
    float dt = 0.f; // For changing of the time
    sf::Clock dtClock;
    sf::RenderWindow window(sf::VideoMode(1920, 1028), "Terrain", sf::Style::Close);
    sf::View view;

    sf::RectangleShape player(sf::Vector2f(gridSizeF,gridSizeF*3.0f));
    player.setFillColor(sf::Color::Red);
    player.setPosition(window.getSize().x/2.f,window.getSize().y);
    // Initialize elements for cursor selection
    sf::Vector2i mouseScreen;
    sf::Vector2i mouseWindow;
    sf::Vector2f mouseView;
    sf::Vector2u mouseGrid;

    // Initialize click elements
    sf::Vector2i clickPos;
    
    // Size of terrain
    const int height = 200;
    const int width = 400;

    // Sets window size
    view.setSize(1500.0f,1080.0f);
    view.setCenter(window.getSize().x/2.f,window.getSize().y);

    // Object that selects a grid
    sf::RectangleShape cursor(sf::Vector2f(gridSizeF,gridSizeF));
    cursor.setFillColor(sf::Color::Transparent);
    cursor.setOutlineThickness(1.0f);
    cursor.setOutlineColor(sf::Color::Red);



    
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

    // We assume that the cave systems begin at half of the world
    // can change this later
    int relativeHeight = height/2;

    // Initialize mountains
    float fnoiseSeed1D[width];
    float fperlinNoise1D[width];

    // These 3 values you can edit to create different terrains
    int octave1D = 7; // 1 - 7, oddly
    float bias = 1.50f;
    float altitude = 4.0f; 


    for(int i = 0; i < width; i++)fnoiseSeed1D[i] = (float)rand()/(float)RAND_MAX;

    perlinNoise1D(width,fnoiseSeed1D, octave1D, fperlinNoise1D, bias);

    for(int x = 0; x < width; x++){
        int y = (fperlinNoise1D[x]* (float)relativeHeight/altitude + (float)relativeHeight/altitude);
        //printf("%d ", y); // This print how high the mountain will be for each column
            tileMap[x][y].setFillColor(sf::Color::Green);

        for(int f = relativeHeight; f > y; f--)
            tileMap[x][f].setFillColor(sf::Color::Black);
    }

    // Values you can edit to generate cave systems
    int octave2D = 2;// 1 - 8 I think
    double zoom = 5.0f, point = 3.0f, scale = 1.0f;

    // Initialize Caves
    for(int x = 0; x < width; x++){
        for(int y = relativeHeight; y < height; y++){
            int color = (int)(noise2d(x,y,octave2D,zoom,point,scale));
            if(color > 120)
                tileMap[x][y].setFillColor(sf::Color::Blue);
            else
                tileMap[x][y].setFillColor(sf::Color::White);
        }
    }

    int color = 0;
    while (window.isOpen()){
        dt = dtClock.restart().asSeconds();

        // Mouse position
        mouseScreen = sf::Mouse::getPosition();
        mouseWindow = sf::Mouse::getPosition(window);
        window.setView(view);
        mouseView = window.mapPixelToCoords(mouseWindow);
        if(mouseView.x >= 0.f)
            mouseGrid.x = mouseView.x / gridSizeU;
        if(mouseView.y >= 0.f)
            mouseGrid.y = mouseView.y / gridSizeU;
        //std::cout << "Grid Position, x: " << mouseGrid.x << " y: " << mouseGrid.y << std::endl;

        // Cursor
        // Snaps cursor to a grid
        cursor.setPosition(mouseGrid.x * gridSizeF, mouseGrid.y * gridSizeF);

        sf::Event ev;

        while(window.pollEvent(ev)){
            switch(ev.type){ 
                case sf::Event::Closed:
                    window.close();
                    break;
                default:
                    break;
            } 

            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
                player.move(-viewSpeed*dt, 0.f);
            else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
                player.move(viewSpeed*dt, 0.f);
            else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
                player.move(0.f,-viewSpeed*dt);
            else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
                player.move(0.f,viewSpeed*dt);
            
            // Move the view
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::A))
                view.move(-viewSpeed*dt, 0.f);
            else if(sf::Keyboard::isKeyPressed(sf::Keyboard::D))
                view.move(viewSpeed*dt, 0.f);
            else if(sf::Keyboard::isKeyPressed(sf::Keyboard::W))
                view.move(0.f,-viewSpeed*dt);
            else if(sf::Keyboard::isKeyPressed(sf::Keyboard::S))
                view.move(0.f,viewSpeed*dt);
            
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::C))
                color += 1;
            
            if(color > 2)
                color = 0;
            // Click the tile

            switch(color){
                case 0:
                    cursor.setOutlineColor(sf::Color::Red);
                    break;
                case 1:
                    cursor.setOutlineColor(sf::Color::Blue);
                    break;
                case 2:
                    cursor.setOutlineColor(sf::Color::Yellow);
                    break;
                default:
                    break;
            }

            if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
                if(tileMap[mouseGrid.x][mouseGrid.y].getFillColor() == sf::Color::White){
                    switch(color){
                        case 0:
                            tileMap[mouseGrid.x][mouseGrid.y].setFillColor(sf::Color::Red);
                            cursor.setOutlineColor(sf::Color::Red);
                            break;
                        case 1:
                        tileMap[mouseGrid.x][mouseGrid.y].setFillColor(sf::Color::Blue);
                        cursor.setOutlineColor(sf::Color::Blue);
                            break;
                        case 2:
                            tileMap[mouseGrid.x][mouseGrid.y].setFillColor(sf::Color::Yellow);
                            cursor.setOutlineColor(sf::Color::Yellow);
                            break;
                        default:
                            break;
                    }
                }
            if(sf::Mouse::isButtonPressed(sf::Mouse::Right))
                tileMap[mouseGrid.x][mouseGrid.y].setFillColor(sf::Color::White);


        }

        window.clear();
        window.setView(view);

        // Draw tilemap
        for(int x = 0; x < width; x++){
            for(int y = 0; y < height; y++)
                window.draw(tileMap[x][y]);
        }

        window.draw(cursor);    
        window.draw(player);

        window.setView(window.getDefaultView());
        window.display();
    }
}

//g++ mountain-caves.cpp -o t -lsfml-graphics -lsfml-window -lsfml-system
