// Benjamin Ramirez
// May 31, 2017
// Simple Breakout clone using the SFML library

#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <unistd.h>
#include <math.h>

#define WIN_WIDTH 800
#define WIN_HEIGHT 500
#define NUM_LAYERS 3
#define NUM_COLS 7
#define BLOCK_OFFSET 50
#define BLOCK_HEIGHT 15
#define PI 3.14159265

struct Rectangle
{
    sf::RectangleShape body;
    float getX(){return body.getPosition().x;}
    float getY(){return body.getPosition().y;}
    float getCenterX(){return getX() + getWidth()/2.f;}
    float getCenterY(){return getY() + getHeight()/2.f;}
    float getLeft(){return getX();}
    float getRight(){return getX() + body.getLocalBounds().width;}
    float getTop(){return getY();}
    float getBottom(){return getY() + body.getLocalBounds().height;}
    float getWidth(){return body.getLocalBounds().width;}
    float getHeight(){return body.getLocalBounds().height;}
};

struct Paddle: public Rectangle
{
    sf::Vector2f velocity;

    Paddle(sf::Vector2f size, float x, float y)
    {
        body.setSize(size);
        body.setPosition(x, y);
        body.setFillColor(sf::Color::White);
    }

    void update()
    {
        body.move(velocity);
        checkBoundaries();
        stop();
    }
    
    void stop(){velocity.y = 0;}

    void checkBoundaries()
    {
        if(getTop() < 0){ body.setPosition(getX(), 0); }
        if(getBottom() > WIN_HEIGHT){ body.setPosition(getX(), WIN_HEIGHT - getHeight()); }
    }

    void move(bool up, bool down)
    {
        if(up) this->velocity.y = -5;
        if(down) this->velocity.y = 5;
    }
};

struct Ball: public Rectangle // a Rectangular Ball...
{
    sf::Vector2f velocity;
    float speed = 6.f;

    Ball(sf::Vector2f size, float x, float y, float angle, float speed)
    {
        body.setSize(size);
        body.setPosition(x, y);
        body.setFillColor(sf::Color::White);
        this->speed = speed;
        setVelocity(angle);
    }

    void update()
    {
        checkBoundaries();
        body.move(velocity);
    }

    void setVelocity(float angle)
    {
        velocity.x = 3 * speed * cos(angle); //bias for x movement
        velocity.y = speed * sin(angle);
    }
    
    void checkBoundaries()
    {
        // left and right wall collisions
        if(getLeft() < 0){body.setPosition(0, getY()); velocity.x *= -1; }
        if(getRight() > WIN_WIDTH){body.setPosition(WIN_WIDTH - getWidth(), getY()); velocity.x *= -1;}
        // top bottom wall collisions
        if(getBottom() > WIN_HEIGHT){body.setPosition(getX(), WIN_HEIGHT - getHeight()); velocity.y *= -1;}
        if(getTop() < 0){body.setPosition(getX(), 0 ); velocity.y *= -1;}
    }
};


void checkPaddleHits(Paddle& paddle, Ball& ball)
{
    // send the ball flying in different directions based on angle of impact
    if(paddle.body.getGlobalBounds().intersects(ball.body.getGlobalBounds()))
    {
        float collision_angle = atan2(ball.getCenterY() - paddle.getCenterY(),
                                      ball.getCenterX() - paddle.getCenterX());
        ball.setVelocity(collision_angle);
    }
}


int main()
{
    sf::RenderWindow window(sf::VideoMode(WIN_WIDTH, WIN_HEIGHT), "Pong Clone");

    // the player rectangle
    sf::Vector2f playerSize(BLOCK_HEIGHT, WIN_HEIGHT/5);
    Paddle player1(playerSize, WIN_WIDTH/8, WIN_HEIGHT - playerSize.y);

    Paddle player2(playerSize, (7.f/8) * (float)WIN_WIDTH, 0);

    // testing out the amazing rectangular ball
    sf::Vector2f ballSize(BLOCK_HEIGHT, BLOCK_HEIGHT);
    Ball ball(ballSize, WIN_WIDTH/2, WIN_HEIGHT/2 , 0, 6.f);

    // set up objects for keeping constant FPS
    window.setFramerateLimit(60);

    std::cout << "Game is starting!\n";

    while (window.isOpen())
    {
        window.clear();
        
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        // handle player1 input
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) && sf::Keyboard::isKeyPressed(sf::Keyboard::S))
            player1.stop();
        else if(sf::Keyboard::isKeyPressed(sf::Keyboard::W))
            player1.move(true, false);
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
            player1.move(false, true);
        //handle player2 input
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::I) && sf::Keyboard::isKeyPressed(sf::Keyboard::K))
            player2.stop();
        else if(sf::Keyboard::isKeyPressed(sf::Keyboard::I))
            player2.move(true, false);
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::K))
            player2.move(false, true);
    
        // collisions
        checkPaddleHits(player1, ball);
        checkPaddleHits(player2, ball);

        player1.update();
        player2.update();
        ball.update();

        // draw the updates
        window.draw(player1.body);
        window.draw(player2.body);
        window.draw(ball.body);

        window.display();
    }

    return 0;
}