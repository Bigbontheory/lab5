#pragma once
#include <SFML/System/Vector2.hpp>
#include "PhysicsScale.hpp"

class FixedCharge {
private:
    sf::Vector2f m_position;
    float m_charge;
    float m_mass;
public:

    FixedCharge() : m_position(0.0f, 0.0f), m_charge(0.0f) {}
    FixedCharge(sf::Vector2f pos, float charge, float mass);

    sf::Vector2f getPosition() const { return m_position; }
    float getCharge() const { return m_charge; }
    float getMass() const { return m_mass; }

    void setCharge(float charge);
    void setMass(float mass) {
        m_mass = static_cast<float>(PhysicsScale::obstacleMassToKg(mass));
    }
};