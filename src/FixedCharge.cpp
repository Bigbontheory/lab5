#include "FixedCharge.hpp"

FixedCharge::FixedCharge(sf::Vector2f pos, float charge, float mass) {
    m_position = PhysicsScale::toPhysics(pos);
    m_charge = PhysicsScale::chargeToCoulombs(charge);
    m_mass = PhysicsScale::obstacleMassToKg(mass);
}

void FixedCharge::setCharge(float charge) {
    m_charge = PhysicsScale::chargeToCoulombs(charge);
}


