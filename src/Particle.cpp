#include "Particle.hpp"
#include "PhysicsScale.hpp"

Particle::Particle(sf::Vector2f pos, sf::Vector2f vel, float mass, float charge) {
    m_position = PhysicsScale::toPhysics(pos);
    m_velocity = PhysicsScale::toPhysics(vel);
    m_mass = PhysicsScale::particleMassToKg(mass);
    m_charge = PhysicsScale::chargeToCoulombs(charge);
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
    m_velocity = PhysicsScale::toPhysics(newVel);
}

void Particle::setPosition(sf::Vector2f newPos) {
    m_position = PhysicsScale::toPhysics(newPos);
}