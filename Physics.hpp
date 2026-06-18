#pragma once
#include <SFML/System/Vector2.hpp>

namespace Physics {
    const float K = 1000.0f;
    const float G = 10.0f;

    // Функция подсчета силы Кулона между неподвижным зарядом Q и летящей частицей q
    sf::Vector2f calculateCoulombForce(sf::Vector2f particlePos, float particleCharge,
        sf::Vector2f chargePos, float fixedCharge);

    // Функция подсчета силы внутри плоского конденсатора
    sf::Vector2f calculateCapacitorForce(sf::Vector2f particlePos, float particleCharge,
        float startX, float endX, float fieldIntensity);

    sf::Vector2f calculateLorentzForce(sf::Vector2f velocity, float charge, float B);

    // Функция подсчета силы всемирного тяготения Ньютона между частицей и мишенью
    sf::Vector2f calculateGravityForce(sf::Vector2f particlePos, float particleMass,
        sf::Vector2f targetPos, float targetMass);
}