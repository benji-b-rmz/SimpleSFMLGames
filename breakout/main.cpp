// Benjamin Ramirez
// May 27, 2017
// Simple Breakout clone using the SFML library

#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <unistd.h>
#include <math.h>

#define WIN_WIDTH 600
#define WIN_HEIGHT 600
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

struct Player: public Rectangle
{
    sf::Vector2f velocity;
    int lives = 5;

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

    void loseLife(){this->lives--;};
    
    bool isAlive()
    {
        if(this->lives > 0)
            return true;
        return false;
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
        velocity.x = speed * cos(angle);
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

    sf::Vector2f blockSize(WIN_WIDTH/NUM_COLS - 2, BLOCK_HEIGHT);
    for(int i = 0; i < NUM_LAYERS; i++)
    {
        for(int j = 0; j < NUM_COLS; j++)
        {
            float x = ((float)j/NUM_COLS) * (float)WIN_WIDTH ;
            float y = BLOCK_OFFSET + ((float)i * BLOCK_HEIGHT) + i;
            Block block(blockSize, x, y);
            blocks.push_back(block);
        }
    }
}

void checkPlayerHits(Player& player, Ball& ball)
{
    // send the ball flying in different directions based on angle of impact
    if(player.body.getGlobalBounds().intersects(ball.body.getGlobalBounds()))
    {
        float collision_angle = atan2(ball.getCenterY() - player.getCenterY(),
                                      ball.getCenterX() - player.getCenterX());
        ball.setVelocity(collision_angle);
    }
}

bool checkBlockHits(Block& block, Ball& ball)
{
    if(!block.alive){return false;} // block is dead, ignore
    if(block.body.getGlobalBounds().intersects(ball.body.getGlobalBounds())) 
    {   // send ball flying in different directions based on angle of impact
        block.alive = false;
        float collision_angle = atan2(ball.getCenterY() - block.getCenterY(),
                                      ball.getCenterX() - block.getCenterX());
        ball.setVelocity(collision_angle);
        return true;
    }
    return false;
}

int main()
{
    sf::RenderWindow window(sf::VideoMode(WIN_WIDTH, WIN_HEIGHT), "Breakout Clone");

    // the player rectangle
    sf::Vector2f playerSize(100.f, BLOCK_HEIGHT);
    Player player(playerSize, WIN_WIDTH/2, WIN_HEIGHT - playerSize.x);

    // testing out the amazing rectangular ball
    sf::Vector2f ballSize(BLOCK_HEIGHT, BLOCK_HEIGHT);
    Ball ball(ballSize, WIN_WIDTH/2, WIN_HEIGHT/2 , PI/2.f, 6.f);

    std::vector<Block> blocks;
    initBlocks(blocks); 

    // set up objects for keeping constant FPS
    window.setFramerateLimit(60);
    int score = 0;  //Holding the current score, increments when block is broken
    // setting up game text
    sf::Font gameFont;
    if(!gameFont.loadFromFile("arial.ttf"))
    { //there was an error loading the text file
        std::cout << "Font did not load\n" << std::endl;
    }
    sf::Text statusText;
    statusText.setFont(gameFont);
    statusText.setString("Score: " + std::to_string(score)); 
    statusText.setCharacterSize(20);
    statusText.setColor(sf::Color::White);
    statusText.setStyle(sf::Text::Bold);

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
        // handle player input
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) && sf::Keyboard::isKeyPressed(sf::Keyboard::D))
            player.stop();
        else if(sf::Keyboard::isKeyPressed(sf::Keyboard::A))
            player.move(true, false);
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
            player.move(false, true);
    
        checkPlayerHits(player, ball);
        player.update();

        for(int i = 0; i < NUM_LAYERS; i++){
            for (int j = 0; j < NUM_COLS; j++){
                if(blocks[i*NUM_COLS + j].alive)
                {
                    window.draw(blocks[i*NUM_COLS + j].body);
                    if(checkBlockHits(blocks[i*NUM_COLS + j], ball))
                        score++;
                        statusText.setString("Score: " + std::to_string(score));
                }
            }
        }
        
        ball.update();
        // draw the updates
        window.draw(player.body);
        window.draw(ball.body);
        window.draw(statusText);

        window.display();
    }

    return 0;
}

