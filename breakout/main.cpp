// Benjamin Ramirez
// May 27, 2017
// Simple Breakout clone using the SFML library

#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <unistd.h>

#define WIN_WIDTH 600
#define WIN_HEIGHT 600
#define NUM_LAYERS 3
#define NUM_COLS 7
#define BLOCK_OFFSET 50
#define BLOCK_HEIGHT 15

struct Rectangle
{
    sf::RectangleShape body;
    float getX(){return body.getPosition().x;}
    float getY(){return body.getPosition().y;}
    float getLeft(){return getX();}
    float getRight(){return getX() + body.getLocalBounds().width;}
    float getTop(){return getY();}
    float getBottom(){return getY() + body.getLocalBounds().height;}
    float getWidth(){return body.getLocalBounds().width;}
    float getHeight(){return body.getLocalBounds().height;}
};

struct Player: public Rectangle
{
    sf::Vector2f velocity;

    Player(sf::Vector2f size, float x, float y)
    {
        body.setSize(size);
        body.setPosition(x, y);
        body.setFillColor(sf::Color::Blue);
    }

    void update()
    {
        body.move(velocity);
        checkBoundaries();
        stop();
    }
    
    void stop(){velocity.x = 0;}

    void checkBoundaries()
    {
        if(getLeft() < 0){ body.setPosition(0, getY()); }
        if(getRight() > WIN_WIDTH){ body.setPosition(WIN_WIDTH - getWidth(), getY()); }
    }

    void move(bool left, bool right)
    {
        if(left) this->velocity.x = -5;
        if(right) this->velocity.x = 5;
    }
};

struct Ball: public Rectangle // a Rectangular Ball...
{
    sf::Vector2f velocity;

    Ball(sf::Vector2f size, float x, float y, float vX, float vY)
    {
        body.setSize(size);
        body.setPosition(x, y);
        body.setFillColor(sf::Color::White);
        velocity.x = vX;
        velocity.y = vY;
    }

    void update()
    {
        checkBoundaries();
        body.move(velocity);
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

struct Block: public Rectangle
{
    bool alive = true;

    Block(sf::Vector2f size, float x, float y)
    {
        body.setSize(size);
        body.setPosition(x, y);
        body.setFillColor(sf::Color::Red);
    }
};

void initBlocks( std::vector<Block>& blocks){

    sf::Vector2f blockSize(WIN_WIDTH/NUM_COLS, BLOCK_HEIGHT);
    for(int i = 0; i < NUM_LAYERS; i++)
    {
        for(int j = 0; j < NUM_COLS; j++)
        {
            float x = ((float)j/NUM_COLS) * (float)WIN_WIDTH + j;
            float y = BLOCK_OFFSET + ((float)i * BLOCK_HEIGHT) + i;
            Block block(blockSize, x, y);
            blocks.push_back(block);
        }
    }
}

int main()
{
    sf::RenderWindow window(sf::VideoMode(WIN_WIDTH, WIN_HEIGHT), "Breakout Clone");

    // the player rectangle
    sf::Vector2f playerSize(100.f, BLOCK_HEIGHT);
    Player player(playerSize, WIN_WIDTH/2, WIN_HEIGHT - playerSize.x);

    // testing out the amazing rectangular ball
    sf::Vector2f ballSize(BLOCK_HEIGHT, BLOCK_HEIGHT);
    Ball ball(ballSize, WIN_WIDTH/2, WIN_HEIGHT/2 , -4.f, 4.f);

    std::vector<Block> blocks;
    initBlocks(blocks); 

    // set up objects for keeping constant FPS
    window.setFramerateLimit(60);

    std::cout << "Game is starting!\n";

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        // handle player input
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) && sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        {
            player.stop();
        }
        else if(sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        {
            player.move(true, false);
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        {
            player.move(false, true);
        }

        player.update();
        ball.update();
        // erase old render
        window.clear();
        // draw the updates
        window.draw(player.body);
        window.draw(ball.body);

        for(int i = 0; i < NUM_LAYERS; i++){
            for (int j = 0; j < NUM_COLS; j++){
                if(blocks[i*NUM_COLS + j].alive)
                    window.draw(blocks[i*NUM_COLS + j].body);
            }
        }

        window.display();
        
    }

    return 0;
}