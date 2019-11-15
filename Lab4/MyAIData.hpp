#ifndef ECE244_GALAXY_EXPLORER_STUDENT_AI_DATA_HPP
#define ECE244_GALAXY_EXPLORER_STUDENT_AI_DATA_HPP

#include <galaxy-explorer/Asteroid.hpp>
#include <galaxy-explorer/AsteroidsObserver.hpp>
#include <galaxy-explorer/GameAI.hpp>

sf::Vector2f getAsteroidDot(const Asteroid& ast);
sf::Vector2f getShipDot(const ShipState& shipState);
int pointAngle (const sf::Vector2f& src, const sf::Vector2f point);
int shootAngle (const Asteroid& asteroid, const ShipState& shipState, const GameInfo& gameInfo);
int asteroidAngle (const Asteroid& asteroid, const ShipState& shipState, bool leftEnd);
const Asteroid* dangerAsteroid (const AsteroidsObserver& asteroidsObserver, const ShipState& shipState);
const Asteroid* bestRoute (const AsteroidsObserver& asteroidsObserver, const ShipState& shipState, const GameInfo& gameInfo);

/**
 * Student editable struct for storing their ai state
 * Yes, you may change this header.
 * Maybe you want to remember the last asteroid Id you shot at?
 */
struct MyAIData {
    bool debug_on;
    bool danger = false;
    bool angleExists = false;
    int angleDestination = 1000;
    int moveDistance = 1;
    int move;
    SuggestedAction::Yawing lastYaw = SuggestedAction::YawingStop;
};

#endif /* ECE244_GALAXY_EXPLORER_STUDENT_AI_DATA_HPP */