#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>

#include "Help.hpp"
#include "Forklift.hpp"
#include "Camera.hpp"
#include "PID.hpp"
#include "Roadmaker.hpp"

/*
PAMIETĘTAJ ŻEBY SIĘ NIE BAWIĆ W MEGA OGÓLNE ROZWIĄZANIA
TO JEST JEDNOZADANIOWY PROGRAM
*/

int main(/* int argc, char *argv[] */)
{
    int width = 1024;
    int height = 512;
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
    // PID controller = PID(10.0f, 0.1f, 1.0f);
    PID controller = PID(10.0f, 0.25f, 4.0f);

    hero.rotateBy(-90.0f);
    hero.moveBy(Vector2f(-35.0f, 0.0f));
    hero.wheels[2].rotation = -5.0f;

    Roadmaker gddkia;

    gddkia.addSegment("tr 0 5");

    gddkia.addSegment("s 20");
    gddkia.addSegment("tl 20 95");
    gddkia.addSegment("s 5");
    gddkia.addSegment("tl 20 180");
    gddkia.addSegment("s 5");
    gddkia.addSegment("tr 20 90");

    gddkia.addSegment("s 25");

    gddkia.addSegment("tr 20 90");
    gddkia.addSegment("s 5");
    gddkia.addSegment("tl 20 180");
    gddkia.addSegment("s 5");
    gddkia.addSegment("tl 20 95");
    gddkia.addSegment("s 20");

    vector<RectangleShape> lines = gddkia.createShapes();

    hero.eyes.lines = lines;

    float error = 0.0f;

    window.setView(sf::View(Vector2f(-30.0f, 0.0f), Vector2f(200.0f, 100.0f) * 2.0f));

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

        sf::View camera = sf::View(hero.getPosition(), Vector2f(200.0f, 100.0f) * 2.0f);

        float timescale = 1.0f;
        float deltaT = delta.restart().asSeconds() * timescale;
        float elapsedT = elapsed.getElapsedTime().asSeconds() * timescale;

        error = hero.eyes.sense(error);

        float steering = controller.update(error, deltaT);

        hero.drive(0.5f, deltaT);
        hero.steeringTarget = steering + 0.00123f;

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

        // std::cout << elapsedT << '\t' << offset << '\n';

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