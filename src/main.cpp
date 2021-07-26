#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
#include <limits>
#include <algorithm>

#include "Help.hpp"
#include "Forklift.hpp"
#include "PID.hpp"
#include "Roadmaker.hpp"

/*
PAMIETĘTAJ ŻEBY SIĘ NIE BAWIĆ W MEGA OGÓLNE ROZWIĄZANIA
TO JEST JEDNOZADANIOWY PROGRAM
*/

int main(/* int argc, char *argv[] */)
{
    int width = 800;
    int height = 800;
    sf::ContextSettings settings;
    settings.antialiasingLevel = 2;
    sf::RenderWindow window(sf::VideoMode(width, height), "Forklift Sim", sf::Style::Default, settings);
    window.setFramerateLimit(60);

    sf::Image gridImage = help::makeGridImage(32, 32);
    sf::Texture gridTexture;
    gridTexture.loadFromImage(gridImage);
    gridTexture.setRepeated(true);
    gridTexture.setSmooth(true);
    sf::Sprite background = sf::Sprite(gridTexture);
    background.setTextureRect(sf::IntRect(0, 0, 4096, 4096));
    background.setScale(Vector2f(1.0f / 0.32f, 1.0f / 0.32f) * 0.1f);
    background.setPosition(Vector2f(-640.0f, -640.0f));

    sf::Clock delta;
    sf::Clock elapsed;

    Forklift hero = Forklift();
    std::unique_ptr<Controller> controller = std::make_unique<PID>(10.0f, 0.25f, 4.0f);

    hero.rotateBy(-90.0f);

    Roadmaker gddkia;
    RoadConfig config{5, 5, 10, 0, 5.0f, 20.0f, 5.0f, 25.0f, 30.0f, 180.0f, 75.0f, true, true};
    vector<RectangleShape> lines;
    float error = 0.0f;
    bool started = false;
    Vector2f finish_point;

    float const fixed_timestep = 0.016f;
    float const target_timewarp = 10.0f;
    float warped_time = 0.0f;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
        }

        float deltaT = delta.restart().asSeconds();
        float elapsedT = elapsed.getElapsedTime().asSeconds();

        if (!started || elapsedT > 6.0f)
        {
            started = true;
            hero = Forklift();
            controller = std::make_unique<PID>(10.0f, 0.25f, 4.0f);

            hero.rotateBy(-90.0f);
            hero.wheels[2].rotation = -5.0f;

            warped_time = 0.0f;

            unsigned int tries = 0;
            do
            {
                gddkia.generateSegments(config);
                lines = gddkia.createShapes();
                finish_point = gddkia.getFinishPoint();
                tries++;
            } while (!help::checkNiceTrack(lines, 7.0f));
            hero.eyes.distance_fields = gddkia.getDistanceFields();
            elapsed.restart();
        }

        sf::View camera = sf::View(hero.getPosition(), Vector2f(200.0f, 200.0f) * 2.0f);
        window.setView(camera);

        int frameskip = std::max(1, (int)std::ceil((target_timewarp * fixed_timestep) / deltaT));
        for (int i = 0; i < frameskip; ++i)
        {
            if (help::distance(hero.getPosition(), finish_point) < 2.0f)
            {
                // squared_error = 0.0f;
                started = false;
                break;
            }

            error = hero.eyes.sense(error);


            float steering = controller->update({error}, fixed_timestep);

            hero.drive(0.5f, fixed_timestep);
            hero.steeringTarget = steering;

            warped_time += fixed_timestep;
        }

        float radius = std::abs(hero.turnRadius());
        float offset = hero.turnOffset();

        window.clear(sf::Color(253, 246, 227));

        window.draw(background);

        for (RectangleShape line : lines)
        {
            window.draw(line);
        }

        float parentRot = hero.getRotation();
        Vector2f parentPos = hero.getPosition();

        Vector2f dotPos = parentPos + help::rotateByDeg(Vector2f(offset, hero.wheels[0].position.y), parentRot);
        hero.dot.setPosition(dotPos);

        sf::CircleShape circle = sf::CircleShape(radius + 0.5f, 720);
        circle.setOrigin(Vector2f(radius + 0.5f, radius + 0.5f));
        circle.setPosition(dotPos);
        circle.setOutlineColor(sf::Color::Magenta);
        circle.setFillColor(sf::Color::Transparent);
        circle.setOutlineThickness(-1.0f);
        window.draw(circle);
        window.draw(hero.dot);

        hero.shape.setPosition(hero.getPosition());
        hero.shape.setRotation(hero.getRotation());
        window.draw(hero.shape);

        for (int i = 0; i < 3; i++)
        {
            Wheel *wheel = &hero.wheels[i];
            wheel->shape.setPosition(parentPos + help::rotateByDeg(wheel->position, parentRot));
            wheel->shape.setRotation(parentRot + wheel->rotation);
            window.draw(wheel->shape);
        }

        for (int i = 0; i < Sensor::width; i++)
        {
            hero.eyes.dots[i].setPosition(parentPos + help::rotateByDeg(hero.eyes.samplePoints[i], parentRot));
            window.draw(hero.eyes.dots[i]);
        }

        window.display();
    }

    return 0;
}
