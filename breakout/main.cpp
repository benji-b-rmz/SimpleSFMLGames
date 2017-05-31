// Benjamin Ramirez
// May 27, 2017
// Simple Breakout clone using the SFML library

#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <unistd.h>

#define WIN_WIDTH 400
#define WIN_HEIGHT 600

#define NUM_LAYERS 3
#define NUM_COLS 7


struct Player
{

    sf::Vector2f velocity;
    sf::RectangleShape body;
    int leftBound;
    int rightBound;
    
    Player(sf::Vector2f size, float x_position, float y_position, int leftBound, int rightBound)
    {    
        this->body.setSize(size);
        this->body.setPosition(x_position, y_position);
        this->leftBound = leftBound;
        this->rightBound = rightBound;
    };

    void update()
    {
        this->body.setPosition(this->body.getPosition() + this->velocity);
        this->checkBoundaries();
        this->stop();
        // std::cout << this->body.getPosition().x << std::endl;
    }

    void checkBoundaries()
    { // keep player inside window
        
        if(this->body.getPosition().x > (this->rightBound - this->body.getLocalBounds().width))
        {
            this->body.setPosition(this->rightBound - this->body.getLocalBounds().width, this->body.getPosition().y);
        
        }
        else if(this->body.getPosition().x < this->leftBound)
        {

            this->body.setPosition(this->leftBound, this->body.getPosition().y);
        }

    }

    void move(bool left, bool right)
    {
        if(left) this->velocity.x = -5;
        if(right) this->velocity.x = 5;
    }

    void stop(){this->velocity.x = 0;}

};


void initBlocks( sf::RectangleShape blocks[][NUM_COLS]){
    // the 2d vector of blocks
    float blockWidth = WIN_WIDTH/NUM_COLS - 1;
    float blockHeight = (WIN_HEIGHT/NUM_LAYERS)/6 -1;
    float vertOffset = 40;
    sf::Vector2f blockSize( blockWidth, blockHeight );
    for(int i = 0; i < NUM_LAYERS; i++)
    {
        for(int j = 0; j < NUM_COLS; j++)
        {
            float x_position = ((float)j/NUM_COLS) * (float)WIN_WIDTH;
            std::cout << j << std::endl;
                        std::cout << x_position << std::endl;

            float y_position = vertOffset + ((float)i * blockHeight) + i;
            blocks[i][j].setSize(blockSize);
            blocks[i][j].setPosition(x_position, y_position);
        }
    }

}

int main()
{
    sf::RenderWindow window(sf::VideoMode(WIN_WIDTH, WIN_HEIGHT), "Breakout Clone");

    float characterWidth = 15.f;
    // the player rectangle
    sf::Vector2f playerSize(100.f, 15.f);
    Player player(playerSize, WIN_WIDTH/2, WIN_HEIGHT - playerSize.x, 0, WIN_WIDTH);

    // the ball rectangle
    sf::Vector2f ballSize(characterWidth, characterWidth);
    sf::RectangleShape ball(ballSize);
    ball.setPosition(WIN_WIDTH/2, WIN_HEIGHT/2 );
    //allocate memory then set block size and positions
    sf::RectangleShape blocks[NUM_LAYERS][NUM_COLS]; 
    initBlocks(blocks); 

    // set up objects for keeping constant FPS
    sf::Clock gameClock;
    long SKIP_TICKS = 10000;



    std::cout << "Game is starting!\n";

    while (window.isOpen())
    {
        long startTime = gameClock.getElapsedTime().asMilliseconds();

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
        // erase old render
        window.clear();
        // draw the updates
        window.draw(player.body);
        window.draw(ball);
        for(int i = 0; i < NUM_LAYERS; i++){
            for (int j = 0; j < NUM_COLS; j++){
                window.draw(blocks[i][j]);
            }
        }
        // display the updates
        window.display();
        
        long endTime = gameClock.getElapsedTime().asMilliseconds();
        usleep(SKIP_TICKS - (endTime - startTime));
    }

    return 0;
}