#pragma once
#include <SFML/System/Vector2.hpp>

class Particle {
private:
    sf::Vector2f m_position;
    sf::Vector2f m_velocity;
    float m_mass;
    float m_charge;

public:
    Particle(sf::Vector2f pos, sf::Vector2f vel, float mass, float charge);

    void update(sf::Vector2f totalForce, float dt);

    void setVelocity(sf::Vector2f newVel);
    void setPosition(sf::Vector2f newPos);

    sf::Vector2f getPosition() const { return m_position; }
    sf::Vector2f getVelocity() const { return m_velocity; }
    float getMass() const { return m_mass; }
    float getCharge() const { return m_charge; }
};