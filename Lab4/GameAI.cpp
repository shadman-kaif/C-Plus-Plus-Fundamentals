#include <galaxy-explorer/GameAI.hpp>
#include <galaxy-explorer/MyAIData.hpp>
#include <SFML/Graphics.hpp>
#include <cmath>

GameAI::GameAI(const GameInfo& game_info, sf::RenderTarget* debug_rt)
{
	this->debug_rt = debug_rt;
	this->game_info = game_info;
	this->asteroid_observer = AsteroidsObserver(this);
	this->my_game_ai = new MyAIData();
}

GameAI::~GameAI() {
	delete my_game_ai;
}

int angle = 40; 

SuggestedAction GameAI::suggestAction(const ShipState& ship_state) {
    debug_rt->clear(sf::Color::Transparent);
    
    if (my_game_ai->debug_on) {
        if (!asteroidsObserver().asteroids().isEmpty()) {
            const sf::IntRect ship_hb = ship_state.hitbox;
            sf::RectangleShape ship_rect(sf::Vector2f(ship_hb.width, ship_hb.height));
            ship_rect.setPosition(ship_hb.left, ship_hb.top);
            ship_rect.setOutlineThickness(33.0f); // if lines are too thin, they won't show up sometimes
            ship_rect.setOutlineColor(sf::Color::Yellow);
            ship_rect.setFillColor(sf::Color::Transparent);
            debug_rt->draw(ship_rect);

        }
    }
        
    const Asteroid* target = dangerAsteroid(asteroid_observer, ship_state);
    // If there is no asteroid to hit, there is no danger
    if (target == nullptr) {
        my_game_ai->danger = false;
    } 
    else {
        my_game_ai->danger = true;
    }
    
    // Command the ship to go either clockwise or counter-clockwise
    if (target == nullptr) {
        my_game_ai->move++;
        if (my_game_ai->move >= my_game_ai->moveDistance) {
            my_game_ai->move = 0;
            if (ship_state.millidegree_rotation > angle * 1000) {
                my_game_ai->lastYaw = SuggestedAction::YawingAntiClockwise;
                return SuggestedAction{ SuggestedAction::YawingAntiClockwise, SuggestedAction::FiringTry 
                };
            } 
            else if (ship_state.millidegree_rotation < angle * -1000) {
                my_game_ai->lastYaw = SuggestedAction::YawingClockwise;
                return SuggestedAction{ SuggestedAction::YawingClockwise, SuggestedAction::FiringTry 
                };
            } 
            else if (my_game_ai->lastYaw == SuggestedAction::YawingStop) {
                my_game_ai->lastYaw = SuggestedAction::YawingClockwise;
                return SuggestedAction{ SuggestedAction::YawingClockwise, SuggestedAction::FiringTry 
                };
            } 
            else {
                return SuggestedAction{ my_game_ai->lastYaw, SuggestedAction::FiringTry 
                };
            }
        } 
        else {
            return SuggestedAction{ SuggestedAction::YawingStop, SuggestedAction::FiringTry };
        }    
    }
    else {
        if (my_game_ai ->debug_on) {
            if (!asteroidsObserver().asteroids().isEmpty()) {
                const sf::IntRect first_ast_hb = target->getCurrentHitbox();
                sf::RectangleShape ast_rect(sf::Vector2f(first_ast_hb.width, first_ast_hb.height));
                ast_rect.setPosition(first_ast_hb.left, first_ast_hb.top);
                ast_rect.setOutlineThickness(33.0f); 
                ast_rect.setOutlineColor(sf::Color::Yellow);
                ast_rect.setFillColor(sf::Color::Transparent);
                debug_rt->draw(ast_rect);
            }
        }
        
        // Only enter if there is danger detected
        if (my_game_ai->danger) {
            int firingAngle_L = asteroidAngle(*target, ship_state, true);
            int firingAngle_R = asteroidAngle(*target, ship_state, false);
            int shipAngle = ship_state.millidegree_rotation;
            if (shipAngle < firingAngle_L) {
                my_game_ai->lastYaw = SuggestedAction::YawingClockwise;
                return SuggestedAction{ SuggestedAction::YawingClockwise, SuggestedAction::FiringTry };
            } 
            // Use if not else if because you want to check both right and left
            if (shipAngle > firingAngle_R) {
                my_game_ai->lastYaw = SuggestedAction::YawingAntiClockwise;
                return SuggestedAction{ SuggestedAction::YawingAntiClockwise, SuggestedAction::FiringTry };
            } 
            else {
                return SuggestedAction{ my_game_ai->lastYaw, SuggestedAction::FiringTry };
            }
        } 
        else {
            int angle = shootAngle(*target, ship_state, game_info);
            int shipAngle = ship_state.millidegree_rotation;
            if (shipAngle < angle) {
                    my_game_ai->lastYaw = SuggestedAction::YawingClockwise;
                    return SuggestedAction{ SuggestedAction::YawingClockwise, SuggestedAction::FiringTry };
                } 
            if (shipAngle > angle) {
                    my_game_ai->lastYaw = SuggestedAction::YawingAntiClockwise;
                    return SuggestedAction{ SuggestedAction::YawingAntiClockwise, SuggestedAction::FiringTry };
                } 
            else {
                    my_game_ai->angleExists = false;
                    return SuggestedAction{ my_game_ai->lastYaw, SuggestedAction::FiringTry };
            }
        }
    }
}

int asteroidAngle (const Asteroid& asteroid, const ShipState& shipState, bool leftEnd) {
    sf::Vector2f forward(0.0, -1.0);
    sf::Vector2f shipDot(shipState.hitbox.left + shipState.hitbox.width/2.0f, shipState.hitbox.top + shipState.hitbox.height/2.0f);
    sf::Vector2f asteroidDot;
    asteroidDot.y = asteroid.getCurrentHitbox().top + asteroid.getCurrentHitbox().height;
    asteroidDot.x = asteroid.getCurrentHitbox().left;
    if (leftEnd == false) {
        asteroidDot.x = asteroidDot.x + asteroid.getCurrentHitbox().width;
    }
    return pointAngle(shipDot, asteroidDot);
}

int pointAngle (const sf::Vector2f& src, const sf::Vector2f point) {
    const sf::Vector2f forward(0.0, -1.0);
    sf::Vector2f srcToPoint ( point.x - src.x, point.y - src.y );
    float dotProduct = srcToPoint.x * forward.x + srcToPoint.y * forward.y;
    float magProduct = (float) std::sqrt(srcToPoint.x * srcToPoint.x + srcToPoint.y * srcToPoint.y);
    float angle = (std::acos(dotProduct / magProduct) * 180.0f * 7.0f) / 22.0f;
    angle = angle*1000;
    if (point.x < src.x) {
        angle = angle * (-1);
    }
    return (int) angle;
}

const Asteroid* dangerAsteroid (const AsteroidsObserver& asteroidsObserver, const ShipState& shipState) {
    if (asteroidsObserver.asteroids().isEmpty()) {
        return nullptr;
    }
    const Asteroid* asteroid = nullptr;
    double maximumDanger = 0;

    sf::Vector2i shipDot( shipState.hitbox.left + shipState.hitbox.width/2, shipState.hitbox.top + shipState.hitbox.height/2 );

    for (const AsteroidListItem* ptr = asteroidsObserver.asteroids().begin(); ptr != asteroidsObserver.asteroids().end(); ptr = ptr->getNext()) {
        Asteroid ast = ptr->getData();
        sf::Vector2i asteroidDot (ast.getCurrentHitbox().left + ast.getCurrentHitbox().width / 2,ast.getCurrentHitbox().top + ast.getCurrentHitbox().height / 2);
        sf::Vector2i velocity = ast.getVelocity();
        for (int i=0; i<3; i++) {
            int hitPoint;
            switch (i) {
                case 0:
                    hitPoint = shipState.hitbox.top;
                    break;
                case 1:
                    hitPoint = shipDot.y;
                    break;
                case 2:
                    hitPoint = shipState.hitbox.top + shipState.hitbox.height;
                    break;
            }
            float t = std::abs(hitPoint - asteroidDot.y) / (float) velocity.y;
            sf::Vector2i finalPos (asteroidDot.x + velocity.x * t, asteroidDot.y + velocity.y * t);
            sf::IntRect finalHitbox (finalPos.x - ast.getCurrentHitbox().width / 2, finalPos.y - ast.getCurrentHitbox().height / 2,ast.getCurrentHitbox().width, ast.getCurrentHitbox().height);
            bool willHit = shipState.hitbox.intersects(finalHitbox);
            if (willHit == true) {
                float danger = 1.0f / t;
                if (danger > maximumDanger) {
                    maximumDanger = danger;
                    asteroid = &ptr->getData();
                }
                break;
            }
        }
    }
    return asteroid;
}

const Asteroid* bestRoute (const AsteroidsObserver& asteroidsObserver, const ShipState& shipState, const GameInfo& gameInfo) {
    if (asteroidsObserver.asteroids().isEmpty())
        return nullptr;
    const Asteroid* asteroid = &asteroidsObserver.asteroids().front();
    float timeToHit = -1.0;

    sf::Vector2f shipDot(shipState.hitbox.left + shipState.hitbox.width/2.0f,shipState.hitbox.top + shipState.hitbox.height/2.0f);

    for (const AsteroidListItem* ptr = asteroidsObserver.asteroids().begin();ptr != asteroidsObserver.asteroids().end();ptr = ptr->getNext()) {
        Asteroid ast = ptr->getData();
        sf::Vector2f asteroidDot (ast.getCurrentHitbox().left + ast.getCurrentHitbox().width / 2,ast.getCurrentHitbox().top + ast.getCurrentHitbox().height / 2);
        int milliangle = shootAngle(ast, shipState, gameInfo);
        float rotationTime = std::abs((float) milliangle / (float) gameInfo.ship_rotation_speed_millidegrees_per_frame);
        sf::Vector2f shipToAst (asteroidDot.x - shipDot.x, asteroidDot.y - shipDot.y);
        float distance = shipToAst.x * shipToAst.x + shipToAst.y * shipToAst.y;
        float shotTime = distance / gameInfo.phaser_pulse_speed;
        float totalTime = shotTime + rotationTime;
        if (timeToHit < 0 || totalTime < timeToHit) {
            timeToHit = totalTime;
            asteroid = &ptr->getData();
        }
    }
    return asteroid;
}

int shootAngle (const Asteroid& asteroid, const ShipState& shipState, const GameInfo& gameInfo) {
    sf::Vector2f shipDot = getShipDot(shipState);
    sf::Vector2f astDot = getAsteroidDot(asteroid);
    int rotationAngle = pointAngle(shipDot, astDot);
    float t = (float) rotationAngle / (float) gameInfo.ship_rotation_speed_millidegrees_per_frame;
    sf::Vector2f newPoint = astDot + (sf::Vector2f) asteroid.getVelocity() * t;
    rotationAngle = pointAngle(shipDot, newPoint);
    sf::Vector2i vel = asteroid.getVelocity();
    int velMag = vel.x * vel.x + vel.y * vel.y;
    velMag = (int) std::sqrt(velMag);
    double correctionFactor = 100;
    float correction = correctionFactor * velMag;
    if (rotationAngle < 0) {
        correction = correction * (-1);
    }
    return rotationAngle + correction;
}

sf::Vector2f getAsteroidDot(const Asteroid& ast) {
    sf::Vector2f asteroidDot (ast.getCurrentHitbox().left + ast.getCurrentHitbox().width / 2,ast.getCurrentHitbox().top + ast.getCurrentHitbox().height / 2);
    return asteroidDot;
}

sf::Vector2f getShipDot(const ShipState& shipState) {
    sf::Vector2f shipDot(shipState.hitbox.left + shipState.hitbox.width/2,shipState.hitbox.top + shipState.hitbox.height/2);
    return shipDot;
}