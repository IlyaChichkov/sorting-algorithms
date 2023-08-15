#include "gtime.h"

sf::Clock ProgramTime::clock; 
sf::Time ProgramTime::elapsedTime;
void ProgramTime::RestartClock(){
    elapsedTime = clock.restart();
}
float ProgramTime::deltaTime(){
    return elapsedTime.asSeconds();
}