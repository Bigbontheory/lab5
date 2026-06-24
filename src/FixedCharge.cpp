#include "FixedCharge.hpp"

FixedCharge::FixedCharge(sf::Vector2f pos, float charge, float mass)
   : m_position(pos), m_charge(charge), m_mass(mass)
{
}

void FixedCharge::setCharge(float charge) {
    m_charge = charge;
}
