// Benjamin Ramirez
// Basic SFML Window with Text and Shapes
#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>

int main()
{
    sf::RenderWindow window(sf::VideoMode(400, 400), "WHAT'S UP");
    // threre are no dedicated classes for regular polygons
    // simply use the sf::CircleShape class and adjust the number of sides for polygon
    
    // Basic Circle
    sf::CircleShape circle(100.f);
    circle.setFillColor(sf::Color::Green);

    // A regular Triangle (radius, sides)
    sf::CircleShape triangle(40.f, 3);
    triangle.setFillColor(sf::Color::Red);
    triangle.setPosition(300.f,50.f);
    // An octagon
    sf::CircleShape octagon(40.f, 8);
    octagon.setFillColor(sf::Color::Blue);
    octagon.setPosition(200.f, 50.f);
    

    std::cout << "Game is starting!\n";

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        // erase old render
        window.clear();
        
        // draw the updates
        window.draw(circle);
        window.draw(triangle);
        window.draw(octagon);

        // display the updates
        window.display();
    }

    return 0;
}