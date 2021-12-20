#include <SFML/Graphics.hpp>

class Ball
{
    public:
    sf::CircleShape * object;

    // velocity
    float x_vel, y_vel;

    // properties
    float radius;
    sf::Color color;

    Ball(int x, float y, float ballradius, sf::Color ballcolor)
    {
        // initialize internal variables
        setVel(-0.5, -0.5);
        radius = ballradius;
        color = ballcolor;

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

    void update(float win_x_lim, float win_y_lim)
    {
        // Left wall & right wall
        if(object->getPosition().x == 0 || object->getPosition().x+2*radius == win_x_lim)
        {
            x_vel = -x_vel;
        }

        // Top wall & Bottom wall
        if(object->getPosition().y == 0 || object->getPosition().y+2*radius == win_y_lim)
        {
            y_vel = -y_vel;
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
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        /////////////////////////////////////////////////
        // Draw here

        // Calculate new ball & bat position
        static int framecount = 0;
        if (framecount == 10)
        {
            ball.update(window_x_sz, window_y_sz);
            framecount = -1;
        }
        framecount++;

        window.draw(*ball.object);

        /////////////////////////////////////////////////
        // End current frame
        window.display();
    }
    return 0;
}