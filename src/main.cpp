#include <SFML/Graphics.hpp>
#include <iostream>

class Paddle
{
    public:
    sf::RectangleShape * object;

    // movestep
    float step;

    // properties
    float x_dim, y_dim;
    sf::Color color;

    Paddle(float x, float y, float paddle_step, sf::Color paddle_colour, float win_x_lim, float win_y_lim)
    {
        step = paddle_step;
        x_dim = x;
        y_dim = y;

        object = new sf::RectangleShape(sf::Vector2f(x_dim,y_dim));
        object->setFillColor(paddle_colour);
        object->setPosition(win_x_lim/2, win_y_lim - y/2);
    }

    ~Paddle()
    {
        delete object;
    }

    void moveLeft()
    {
        if(object->getPosition().x != 0)
            object->setPosition(object->getPosition().x - step, object->getPosition().y);
    }

    void moveRight(float win_y_lim)
    {
        if(object->getPosition().x+object->getSize().x != win_y_lim)
            object->setPosition(object->getPosition().x + step, object->getPosition().y);
    }
};


class Ball
{
    public:
    sf::CircleShape * object;

    // velocity
    float x_vel, y_vel;

    // properties
    float radius;
    sf::Color color;

    Ball(int x, float y, float ball_radius, sf::Color ball_color)
    {
        // initialize internal variables
        setVel(-0.5, -0.5);
        radius = ball_radius;
        color = ball_color;

        // initialize ball object
        object = new sf::CircleShape(radius);
        object->setFillColor(color);
        object->setPosition(x, y);
    }

    ~Ball()
    {
        delete object;
    }


    void setPos(float x, float y)
    {
        object->setPosition(x, y);
    }

    void setVel(float x, float y)
    {
        x_vel = x;
        y_vel = y;
    }

    void update(float win_x_lim, float win_y_lim, Paddle * bat, sf::RenderWindow * win)
    {
        // Left wall & right wall
        if(object->getPosition().x == 0 || object->getPosition().x+2*radius == win_x_lim)
        {
            x_vel = -x_vel;
        }

        // Top wall
        else if(object->getPosition().y == 0)
        {
            y_vel = -y_vel;
        }

        // Paddle
        else if (object->getPosition().y+2*radius == bat->object->getPosition().y)
        {
            if((object->getPosition().x+radius > bat->object->getPosition().x) && (object->getPosition().x+radius <= bat->object->getPosition().x+bat->x_dim))
            {
                y_vel = -y_vel;
            }
        }
        else if (object->getPosition().y+2*radius == win_y_lim)
        {
            win->close();
        }

        // update pos
        float new_x = object->getPosition().x + x_vel;
        float new_y = object->getPosition().y + y_vel;
        object->setPosition(new_x, new_y);
    }
};


int main()
{
    /////////////////////////////////////////////////////
    // initialize variables
    int window_x_sz = 800, window_y_sz = 600;
    Ball ball(window_x_sz/2, window_y_sz/2, 15, sf::Color::Green);
    Paddle bat(100, 15, 10, sf::Color::Yellow, window_x_sz, window_y_sz);

    /////////////////////////////////////////////////////
    // create the window
    sf::RenderWindow window(sf::VideoMode(window_x_sz, window_y_sz), "Pong");

    // run the program as long as the window is open
    while (window.isOpen())
    {
        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (window.pollEvent(event))
        {
            switch(event.type)
            {
                case sf::Event::Closed:
                    window.close();
                    break;
                
                case sf::Event::KeyPressed:
                    if(event.key.code == sf::Keyboard::Left)
                    {
                        bat.moveLeft();
                    }
                    else if(event.key.code == sf::Keyboard::Right)
                    {
                        bat.moveRight(window_x_sz);
                    }
                    else if(event.key.code == sf::Keyboard::Escape)
                        window.close();
                    break;

                default:
                    break;
            }
        }

        window.clear();
        /////////////////////////////////////////////////
        // Draw here

        // Calculate new ball & bat position
        static int framecount = 0;
        if (framecount == 10)
        {
            ball.update(window_x_sz, window_y_sz, &bat, &window);
            framecount = -1;
        }
        framecount++;

        window.draw(*ball.object);
        window.draw(*bat.object);

        /////////////////////////////////////////////////
        // End current frame
        window.display();
    }
    return 0;
}