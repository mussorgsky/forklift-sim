#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
#include <limits>
#include <algorithm>
#include <fstream>

#include "cppflow/cppflow.h"
#include "Help.hpp"
#include "Forklift.hpp"
#include "PID.hpp"
#include "NNController.hpp"
#include "Roadmaker.hpp"

/*
PAMIETĘTAJ ŻEBY SIĘ NIE BAWIĆ W MEGA OGÓLNE ROZWIĄZANIA
TO JEST JEDNOZADANIOWY PROGRAM
*/

float calculateLoss(std::shared_ptr<Controller> const &controller, std::vector<RoadSegment> const &road_segments);

int main(int argc, char *argv[])
{
    if (argc == 3)
    {
        std::string const operation(argv[1]);
        if (operation == "--generate")
        {
            std::string const config_filename(argv[2]);
            std::ifstream config_file;
            config_file.open(config_filename, std::ios::in);
            if (config_file.good())
            {
                bool failed = false;
                GenerationConfig gen_conf = Roadmaker::loadGenerationConfig(config_file, failed);
                config_file.close();

                if (failed)
                {
                    std::cout << "Generation config file is not correct.\n";
                    return 1;
                }

                Roadmaker roadmaker;
                vector<Vector2f> road_path;
                vector<vector<RoadSegment>> roads;
                for (unsigned int i = 0; i < gen_conf.track_count; ++i)
                {
                    do
                    {
                        roadmaker.generateSegments(gen_conf.config);
                        roadmaker.createShapes();
                    } while (!help::checkNiceTrack(roadmaker.getRoadPath(), roadmaker.getDistanceFields()));

                    roads.push_back(roadmaker.getRoadSegments());
                }
                if (!help::saveToFile(roads, gen_conf.name))
                {
                    std::cout << "Cannot save generated roads.\n";
                }

                return 0;
            }
            else
            {
                std::cout << "Cannot open configuration file.\n";
                return 1;
            }
        }
    }
    if (argc == 4)
    {
        std::string const operation(argv[1]);
        if (operation == "test")
        {
            std::string const model_filename(argv[2]);
            std::string const roads_filename(argv[3]);

            cppflow::model model(model_filename);
            std::shared_ptr<Controller> controller = std::make_shared<NNController>(model);
            std::vector<std::vector<RoadSegment>> rsv = help::loadFromFile(roads_filename);

            std::vector<float> losses;
            for (auto &road_segments : rsv)
            {
                losses.push_back(calculateLoss(controller, road_segments));
            }

            std::ofstream loss_file;
            loss_file.open("losses", std::ios::out | std::ios::trunc);
            if (loss_file.good())
            {
                for (auto loss : losses)
                {
                    loss_file << loss << "\n";
                }
            }
            loss_file.close();

            return 0;
        }
    }
    if (argc != 1)
    {
        std::cout << "Invalid arguments.\n";
        return 1;
    }
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

    Forklift hero;
    std::unique_ptr<Controller> controller;

    hero.rotateBy(-90.0f);

    Roadmaker gddkia;
    RoadConfig config{5, 5, 10, 0, 5.0f, 20.0f, 5.0f, 25.0f, 30.0f, 180.0f, 75.0f, true, true};
    vector<Vector2f> road_path;
    float error = 0.0f;
    bool started = false;
    Vector2f finish_point;
    float squared_error = 0.0f;

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

        if (!started || warped_time > 120.0f)
        {
            started = true;
            hero = Forklift();
            controller = std::make_unique<PID>(12.0f, 0.25f, 4.0f);

            hero.rotateBy(-90.0f);
            hero.wheels[2].rotation = -5.0f;

            warped_time = 0.0f;

            unsigned int tries = 0;
            do
            {
                gddkia.generateSegments(config);
                gddkia.createShapes();
                road_path = gddkia.getRoadPath();
                finish_point = gddkia.getFinishPoint();
                tries++;
            } while (!help::checkNiceTrack(road_path, gddkia.getDistanceFields()));
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
                std::cout << squared_error / (warped_time / fixed_timestep) << "\n";
                squared_error = 0.0f;
                started = false;
                break;
            }

            error = hero.eyes.sense(error);

            float raw_error_squared = std::numeric_limits<float>::max();
            for (auto &df : gddkia.getDistanceFields())
            {
                raw_error_squared = std::min((float)std::pow(df->distance(hero.getPosition()), 2), raw_error_squared);
            }
            squared_error += raw_error_squared;

            float steering = controller->update({error}, fixed_timestep);

            hero.drive(0.5f, fixed_timestep);
            hero.steeringTarget = steering;

            warped_time += fixed_timestep;
        }

        float radius = std::abs(hero.turnRadius());
        float offset = hero.turnOffset();

        window.clear(sf::Color(253, 246, 227));

        window.draw(background);

        vector<vector<Vertex>> road_vertices = gddkia.getRoadVertices();
        for (auto &rv : road_vertices)
        {
            window.draw(&rv[0], rv.size(), sf::TriangleStrip);
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

float calculateLoss(std::shared_ptr<Controller> const &controller, std::vector<RoadSegment> const &road_segments)
{
    float error = 0.0f;
    bool started = false;
    Vector2f finish_point;
    float squared_error = 0.0f;
    float steering = 0.0f;

    float const fixed_timestep = 0.01f;
    float warped_time = 0.0f;

    Forklift forky;

    forky.rotateBy(-90.0f);

    std::vector<float> error_memory(200);

    warped_time = 0.0f;
    Roadmaker gddkia;
    for (auto &rs : road_segments)
    {
        gddkia.addSegment(rs);
    }
    gddkia.createShapes();
    finish_point = gddkia.getFinishPoint();
    forky.eyes.distance_fields = gddkia.getDistanceFields();

    while (warped_time < 120.0f)
    {
        error = forky.eyes.sense(error);
        error_memory.pop_back();
        error_memory.insert(error_memory.begin(), error);

        float raw_error_squared = std::numeric_limits<float>::max();
        for (auto &df : gddkia.getDistanceFields())
        {
            raw_error_squared = std::min((float)std::pow(df->distance(forky.getPosition()), 2), raw_error_squared);
        }
        squared_error += raw_error_squared;

        steering = controller->update({error,
                                       error_memory,
                                       steering,
                                       forky.getWheelAngle()},
                                      fixed_timestep);

        forky.drive(0.5f, fixed_timestep);
        forky.steeringTarget = steering;

        warped_time += fixed_timestep;

        if (help::distance(forky.getPosition(), finish_point) < 2.0f)
        {
            float mean_squared_error = squared_error / (warped_time / fixed_timestep);
            std::cout << mean_squared_error << "\n";
            return mean_squared_error;
        }
    }

    float mean_squared_error = squared_error / (warped_time / fixed_timestep);
    std::cout << mean_squared_error << "\n";
    return mean_squared_error;
}
