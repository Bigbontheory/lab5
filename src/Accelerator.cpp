#include "Accelerator.hpp"

Accelerator::Accelerator(float& capIntensity, float& frequency,
    int numStages, float startX, float baseWidth,
    float stageGap, float growthFactor, float plateHeight,
    float holeSize, float centerY)
    : m_capIntensity(capIntensity)
    , m_frequency(frequency)
    , m_numStages(numStages)
    , m_startX(startX)
    , m_baseWidth(baseWidth)
    , m_stageGap(stageGap)
    , m_growthFactor(growthFactor)
    , m_plateHeight(plateHeight)
    , m_holeSize(holeSize)
    , m_centerY(centerY)
{
    calculateEndPosition();
}

void Accelerator::calculateEndPosition() {
    float currentX = m_startX;
    for (int i = 0; i < m_numStages; ++i) {
        float currentWidth = m_baseWidth * std::pow(m_growthFactor, i);
        currentX += currentWidth + m_stageGap;
    }
    m_acceleratorEndX = currentX;
}

bool Accelerator::updateFieldForce(const sf::Vector2f& particlePos, float particleCharge, float totalTime, sf::Vector2f& outForce) {
    bool insideAnyCapacitor = false;
    float checkX = m_startX;

    for (int i = 0; i < m_numStages; ++i) {
        float currentWidth = m_baseWidth * std::pow(m_growthFactor, i);
        float capLeft = checkX;
        float capRight = capLeft + currentWidth;

        if (particlePos.x >= capLeft && particlePos.x <= capRight) {
            insideAnyCapacitor = true;
            float phaseShift = i * 3.14159f;
            float currentField = m_capIntensity * std::sin(m_frequency * totalTime + phaseShift);


            outForce.x = currentField * particleCharge;
            break;
        }
        checkX = capRight + m_stageGap;
    }
    return insideAnyCapacitor;
}
