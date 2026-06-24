#pragma once
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics.hpp>

class FixedCharge {
private:
    sf::Vector2f m_position; 
    float m_charge;
    float m_mass;

    sf::CircleShape m_shape;

public:
    
    FixedCharge() = default;
    FixedCharge(sf::Vector2f pos, float charge, float mass);

    void draw(sf::RenderTarget& target) const;

    sf::Vector2f getPosition() const { return m_position; }
    float getCharge() const { return m_charge; }
    void setCharge(float newCharge);
    void setPosition(const sf::Vector2f& newPos) { m_position = newPos; }
    float getMass() const { return m_mass; }
    void setMass(float new_mass) { m_mass = new_mass; }
};


