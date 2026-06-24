#include "FixedCharge.hpp"

FixedCharge::FixedCharge(sf::Vector2f pos, float charge, float mass)
    : m_position(pos), m_charge(charge), m_mass(mass)
{
    float radius = 6.0f;
    m_shape.setRadius(radius);
    m_shape.setOrigin(radius, radius);
    m_shape.setPosition(m_position);

    if (m_charge > 0) {
        m_shape.setFillColor(sf::Color(255, 50, 50));
    }
    else {
        m_shape.setFillColor(sf::Color(50, 50, 255));
    }

    m_shape.setOutlineThickness(2.0f);
    m_shape.setOutlineColor(sf::Color::White);
}

void FixedCharge::setCharge(float charge) {
    m_charge = charge;

    if (m_charge > 0.0f) {

        m_shape.setFillColor(sf::Color(255, 50, 50));
        m_shape.setOutlineColor(sf::Color(255, 150, 150));
        m_shape.setOutlineThickness(2.0f);
    }
    else if (m_charge < 0.0f) {
        m_shape.setFillColor(sf::Color(50, 50, 255));
        m_shape.setOutlineColor(sf::Color(150, 150, 255));
        m_shape.setOutlineThickness(2.0f);
    }
    else {
        m_shape.setFillColor(sf::Color(120, 120, 120));
        m_shape.setOutlineThickness(0.0f);
    }
}

void FixedCharge::draw(sf::RenderTarget& target) const {

    if (std::abs(m_charge) > 0.01f) {
        float glowRadius = 20.0f;

        sf::CircleShape glow(glowRadius);
        glow.setOrigin(glowRadius, glowRadius);
        glow.setPosition(m_position);

        if (m_charge > 0.0f) {
            glow.setFillColor(sf::Color(255, 60, 60, 45));
        }
        else {
            glow.setFillColor(sf::Color(60, 60, 255, 45));
        }

        target.draw(glow);
    }

    target.draw(m_shape);
}