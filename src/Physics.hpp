#pragma once
#include <SFML/System/Vector2.hpp>

namespace Physics {
   sf::Vector2f calculateCoulombForce(sf::Vector2f particlePos, float particleCharge,
        sf::Vector2f chargePos, float fixedCharge);

    sf::Vector2f calculateGravityForce(sf::Vector2f particlePos, float particleMass,
        sf::Vector2f targetPos, float targetMass);

    sf::Vector2f calculateLorentzForce(float q, float B, const sf::Vector2f& vel);
}
