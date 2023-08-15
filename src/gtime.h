#pragma once
#include <SFML/Graphics.hpp>

class ProgramTime{
private:

public:
    static sf::Clock clock; 
    static sf::Time elapsedTime;
    static void RestartClock();
    static float deltaTime();
};