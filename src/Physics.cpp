#include "Physics.hpp"
#include <cmath>

sf::Vector2f Physics::calculateCoulombForce(sf::Vector2f particlePos, float particleCharge,
    sf::Vector2f chargePos, float fixedCharge)
{
    sf::Vector2f r = particlePos - chargePos;

    float distanceSq = r.x * r.x + r.y * r.y;

    if (distanceSq < 100.0f) {
        distanceSq = 100.0f;
    }


    float distance = std::sqrt(distanceSq);

    float forceMagnitude = (K * particleCharge * fixedCharge) / distanceSq;

    sf::Vector2f forceVector = forceMagnitude * (r / distance);

    return forceVector;
}




sf::Vector2f Physics::calculateGravityForce(sf::Vector2f particlePos, float particleMass,
    sf::Vector2f targetPos, float targetMass)
{
    sf::Vector2f r = particlePos - targetPos;

    float distanceSq = r.x * r.x + r.y * r.y;


    if (distanceSq < 100.0f) {
        distanceSq = 100.0f;
    }

    float distance = std::sqrt(distanceSq);

    float forceMagnitude = (G * particleMass * targetMass) / distanceSq;

    sf::Vector2f forceVector = -forceMagnitude * (r / distance);

    return forceVector;
}