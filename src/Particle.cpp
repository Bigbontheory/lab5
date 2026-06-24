#include "Particle.hpp"
#include <SFML/Graphics/RenderTarget.hpp>

Particle::Particle(sf::Vector2f pos, sf::Vector2f vel, float mass, float charge)
    : m_position(pos), m_velocity(vel), m_mass(mass), m_charge(charge)
{
}

void Particle::update(sf::Vector2f totalForce, float dt) {
    sf::Vector2f acceleration(0.0f, 0.0f);
    if (m_mass > 0.0f) {
        acceleration = totalForce / m_mass;
    }

    m_velocity += acceleration * dt;

    m_position += m_velocity * dt;

}


void Particle::setVelocity(sf::Vector2f newVel) {
    m_velocity = newVel;
}

void Particle::setPosition(sf::Vector2f newPos) {
    m_position = newPos;
}