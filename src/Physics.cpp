#include "Physics.hpp"
#include "PhysicsScale.hpp"
#include <cmath>

sf::Vector2f Physics::calculateCoulombForce(sf::Vector2f particlePos, float particleCharge,
    sf::Vector2f chargePos, float fixedCharge)
{
    sf::Vector2f r = particlePos - chargePos;
    float distanceSq = r.x * r.x + r.y * r.y;

    if (distanceSq < 0.01f) {
        distanceSq = 0.01f;
    }

    float distance = std::sqrt(distanceSq);
    float forceMagnitude = (static_cast<float>(PhysicsScale::K) * particleCharge * fixedCharge) / distanceSq;
    sf::Vector2f forceVector = forceMagnitude * (r / distance);

    return forceVector;
}

sf::Vector2f Physics::calculateGravityForce(sf::Vector2f particlePos, float particleMass,
    sf::Vector2f targetPos, float targetMass)
{
    sf::Vector2f r = particlePos - targetPos;
    float distanceSq = r.x * r.x + r.y * r.y;

    if (distanceSq < 0.01f) {
        distanceSq = 0.01f;
    }

    float distance = std::sqrt(distanceSq);
    float forceMagnitude = (static_cast<float>(PhysicsScale::G) * particleMass * targetMass) / distanceSq;
    sf::Vector2f forceVector = -forceMagnitude * (r / distance);

    return forceVector;
}

sf::Vector2f Physics:: calculateLorentzForce(float q, float B, const sf::Vector2f& vel) {
    return sf::Vector2f(q * vel.y * B, -q * vel.x * B);
}