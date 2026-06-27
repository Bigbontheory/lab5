#pragma once
#include <SFML/System/Vector2.hpp>

class PhysicsScale {
public:
    static constexpr float METERS_TO_PIXELS = 100.0f;

    static sf::Vector2f toPixels(const sf::Vector2f& physicsPos) {
        return physicsPos * METERS_TO_PIXELS;
    }
    static float toPixels(float physicsLength) {
        return physicsLength * METERS_TO_PIXELS;
    }
    static sf::Vector2f toPhysics(const sf::Vector2f& screenPos) {
        return screenPos / METERS_TO_PIXELS;
    }
    static float toPhysics(float screenLength) {
        return screenLength / METERS_TO_PIXELS;
    }

    static constexpr double G = 6.67430e-11;
    static constexpr double K = 8.98755e9;

    static float particleMassToKg(float uiMass) {
        return uiMass;
    }

    static float obstacleMassToKg(float uiMass) {
        return uiMass * 1e9;
    }

    static float chargeToCoulombs(float uiCharge) {
        return uiCharge * 1e-6f;
    }

    static float voltageToVolts(float uiVoltage) {
        return uiVoltage * 1e3f;
    }
};