#pragma once

#include <cmath>
#include "../LAB2/mutable_array_sequence.hpp"
#include <SFML/System/Vector2.hpp>

class Accelerator {
private:
    int m_numStages;
    float m_startX;
    float m_baseWidth;
    float m_stageGap;
    float m_growthFactor;
    float m_plateHeight;
    float m_holeSize;
    float m_centerY;

    float m_angle;
    sf::Vector2f m_center;

    float& m_capIntensity;
    float& m_frequency;

    float m_acceleratorEndX;
    void calculateEndPosition();

public:
    Accelerator(float& capIntensity, float& frequency,
        int numStages = 5, float startX = 40.0f, float baseWidth = 8.0f,
        float stageGap = 6.0f, float growthFactor = 1.60f, float plateHeight = 80.0f,
        float holeSize = 20.0f, float centerY = 280.0f);

    ~Accelerator() = default;

    void setRotation(float angle);
    float getRotation() const { return m_angle; }
    void setCenter(sf::Vector2f center) { m_center = center; }
    sf::Vector2f getCenter() const { return m_center; }

    sf::Vector2f rotatePoint(sf::Vector2f point, bool inverse = false) const;

    float getStartX() const { return m_startX; }
    float getEndX() const { return m_acceleratorEndX; }
    int getNumStages() const { return m_numStages; }
    float getStageGap() const { return m_stageGap; }
    float getCapIntensity() const { return m_capIntensity; }
    float getFrequency() const { return m_frequency; }
    float getCenterY() const { return m_centerY; }
    float getPlateHeight() const { return m_plateHeight; }
    float getHoleSize() const { return m_holeSize; }
    float getBaseWidth() const { return m_baseWidth; }
    float getGrowthFactor() const { return m_growthFactor; }

    bool updateFieldForce(const sf::Vector2f& particlePos, float particleCharge, float totalTime, sf::Vector2f& outForce);
};