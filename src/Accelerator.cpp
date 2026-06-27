#include "Accelerator.hpp"
#include "PhysicsScale.hpp"
#include <cmath> 

Accelerator::Accelerator(float& capIntensity, float& frequency,
    int numStages, float startX, float baseWidth,
    float stageGap, float growthFactor, float plateHeight,
    float holeSize, float centerY)
    : m_capIntensity(capIntensity)
    , m_frequency(frequency) 
    , m_numStages(numStages)
    , m_startX(PhysicsScale::toPhysics(startX))
    , m_baseWidth(PhysicsScale::toPhysics(baseWidth))
    , m_stageGap(PhysicsScale::toPhysics(stageGap))
    , m_growthFactor(growthFactor)
    , m_plateHeight(PhysicsScale::toPhysics(plateHeight))
    , m_holeSize(PhysicsScale::toPhysics(holeSize))
    , m_centerY(PhysicsScale::toPhysics(centerY))
    , m_angle(0.0f)
    , m_center(0.0f, 0.0f)
{
    calculateEndPosition();

    m_center = sf::Vector2f(m_startX, m_centerY);
}

void Accelerator::setRotation(float angle) {
    m_angle = angle;
}

void Accelerator::calculateEndPosition() {
    float currentX = m_startX;
    for (int i = 0; i < m_numStages; ++i) {
        float currentWidth = m_baseWidth * std::pow(m_growthFactor, i);
        currentX += currentWidth + m_stageGap;
    }
    m_acceleratorEndX = currentX;
}

sf::Vector2f Accelerator::rotatePoint(sf::Vector2f point, bool inverse) const {
    float angleRad = m_angle * (3.14159265f / 180.0f);

    if (!inverse) {
        angleRad = -angleRad;
    }

    float cosA = std::cos(angleRad);
    float sinA = std::sin(angleRad);

    point -= m_center;

    float xNew = point.x * cosA - point.y * sinA;
    float yNew = point.x * sinA + point.y * cosA;

    return sf::Vector2f(xNew + m_center.x, yNew + m_center.y);
}

bool Accelerator::updateFieldForce(const sf::Vector2f& particlePos, float particleCharge, float totalTime, sf::Vector2f& outForce) {
    sf::Vector2f localPos = rotatePoint(particlePos, false);
    outForce = sf::Vector2f(0.0f, 0.0f);

    bool insideAnyCapacitor = false;
    float checkX = m_startX;
    float physIntensity = PhysicsScale::voltageToVolts(m_capIntensity);
    sf::Vector2f localForce(0.0f, 0.0f);

    for (int i = 0; i < m_numStages; ++i) {
        float currentWidth = m_baseWidth * std::pow(m_growthFactor, i);
        float capLeft = checkX;
        float capRight = capLeft + currentWidth;

        if (localPos.x >= capLeft && localPos.x <= capRight) {
            insideAnyCapacitor = true;
            float phaseShift = i * 3.14159f;
            float currentField = physIntensity * std::sin(m_frequency * totalTime + phaseShift);

            localForce.x = currentField * particleCharge;
            localForce.y = 0.0f;
            break;
        }
        checkX = capRight; 
    }

    if (insideAnyCapacitor) {
        float angleRad = m_angle * (3.14159265f / 180.0f);
        float cosA = std::cos(angleRad);
        float sinA = std::sin(angleRad);

        outForce.x = localForce.x * cosA;
        outForce.y = localForce.x * sinA;
    }

    return insideAnyCapacitor;
}