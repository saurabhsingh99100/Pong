#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <string>

class Paddle
{
    public:
    sf::RectangleShape * object;

    // movestep
    float step;

    // properties
    float x_dim, y_dim;
    sf::Color color;

    // Texture
    sf::Texture bat_texture;

    Paddle(float x, float y, float paddle_step, sf::Color paddle_colour, float win_x_lim, float win_y_lim)
    {
        step = paddle_step;
        x_dim = x;
        y_dim = y;

        object = new sf::RectangleShape(sf::Vector2f(x_dim,y_dim));
        object->setFillColor(paddle_colour);
        object->setPosition(win_x_lim/2, win_y_lim - y/2);

        // Texture
        if(!bat_texture.loadFromFile("include/wood-texture.png"))
        {
            std::cerr << "Unable to load: include/wood-texture.png\n";
            return;
        }
        object->setTexture(&bat_texture);
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

    // Sounds
    sf::SoundBuffer buffer_ball_wall_collision;
    sf::SoundBuffer buffer_ball_bat_collision;
    sf::Sound sound_ball_wall_collision;
    sf::Sound sound_ball_bat_collision;

    // Texture
    sf::Texture ball_texture;

    Ball(int x, float y, float ball_radius, sf::Color ball_color)
    {
        // initialize internal variables
        setVel(-0.5, -0.5);
        radius = ball_radius;
        color = ball_color;

        // initialize ball object
        object = new sf::CircleShape(radius);
        //object->setFillColor(color);
        object->setPosition(x, y);

        if(!ball_texture.loadFromFile("include/ball.png"))
        {
            std::cerr << "Unable to load: include/ball.png\n";
            return;
        }
        object->setTexture(&ball_texture);


        // Load sounds
        std::cout << "Loading Sounds...   ";
        if (!buffer_ball_wall_collision.loadFromFile("include/ball-collision1.ogg"))
        {   
            std::cerr << "\nUnable to load : include/ball-collision1.ogg" << std::endl;
            return;
        }
        if (!buffer_ball_bat_collision.loadFromFile("include/ball-collision2.ogg"))
        {
            std::cerr << "\nUnable to load : include/ball-collision2.ogg" << std::endl;
            return;
        }

        sound_ball_wall_collision.setBuffer(buffer_ball_wall_collision);
        sound_ball_bat_collision.setBuffer(buffer_ball_bat_collision);
        std::cout << "Finished!\n";
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

    int update(float win_x_lim, float win_y_lim, Paddle * bat, sf::RenderWindow * win, unsigned int *score)
    {
        // Left wall & right wall
        if(object->getPosition().x <= 0 || object->getPosition().x+2*radius >= win_x_lim)
        {
            x_vel = -x_vel;
            sound_ball_wall_collision.play();   // play sound
        }

        // Top wall
        else if(object->getPosition().y <= 0)
        {
            y_vel = -y_vel;
            sound_ball_wall_collision.play();   // play sound
        }

        // Paddle
        else if ((object->getPosition().y+2*radius >= bat->object->getPosition().y) && (object->getPosition().y+2*radius <= win_y_lim))
        {
            if((object->getPosition().x+radius > bat->object->getPosition().x) && (object->getPosition().x+radius <= bat->object->getPosition().x+bat->x_dim))
            {
                y_vel = -y_vel;
                sound_ball_bat_collision.play();    // play sound
                *score+=10;

                if(x_vel<0)
                    x_vel-=0.1;
                else
                    x_vel+=0.1;
                
                if(y_vel<0)
                    y_vel-=0.1;
                else
                    y_vel+=0.1;

            }
        }
        else if (object->getPosition().y+2*radius > win_y_lim)
        {
            return 1;
        }

        // update pos
        float new_x = object->getPosition().x + x_vel;
        float new_y = object->getPosition().y + y_vel;
        object->setPosition(new_x, new_y);
        return 0;
    }
};


int main()
{
    /////////////////////////////////////////////////////
    // initialize variables
    int window_x_sz = 800, window_y_sz = 600;
    Ball ball(window_x_sz/2, window_y_sz/2, 15, sf::Color::Green);
    Paddle bat(100, 15, 10, sf::Color::Yellow, window_x_sz, window_y_sz);

    unsigned int score=0;

    /////////////////////////////////////////////////////
    // create the window
    sf::RenderWindow window(sf::VideoMode(window_x_sz, window_y_sz), "Pong");

    // Add background to window
    sf::Texture textureBackground;   
    textureBackground.loadFromFile("include/background.png");   
    sf::Sprite spriteBackground;   
    spriteBackground.setTexture(textureBackground);   
    spriteBackground.setPosition(0,0);

    // Add scoreboard
    sf::Text scoreboard;
    sf::Font sos_font;
    if (!sos_font.loadFromFile("include/Calibri_Bold.ttf"))
    {
        std::cerr << "error loading font file: include/Calibri_Bold.ttf" << std::endl;
    }
    scoreboard.setFont(sos_font); // font is a sf::Font
    scoreboard.setCharacterSize(24); // in pixels, not points!
    scoreboard.setFillColor(sf::Color::Red);
    scoreboard.setStyle(sf::Text::Bold);
    scoreboard.setPosition(10, 10);

    // Play music
    sf::Music music;
    if (!music.openFromFile("include/Hotel_California_8-Bit.ogg"))
        return -1; // error
    music.play();

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
            if(ball.update(window_x_sz, window_y_sz, &bat, &window, &score) == 1)
            {
                // Game over
                sf::Music game_over_music;
                if (!game_over_music.openFromFile("include/game-over.ogg"))
                {
                    std::cerr << "Unable to load: include/game-over.ogg"<< std::endl;
                    return 1;
                }

                music.stop();
                game_over_music.play();
                
                while(game_over_music.getStatus() == sf::Music::Playing);
                window.close();
            }
            framecount = -1;
        }
        framecount++;

        window.draw(spriteBackground);
        window.draw(*ball.object);
        window.draw(*bat.object);

        scoreboard.setString("Score: "+std::to_string(score));
        window.draw(scoreboard);

        /////////////////////////////////////////////////
        // End current frame
        window.display();
    }
    std::cout << "Final Score: "<<score<<std::endl;

    return 0;
}