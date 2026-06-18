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

void Accelerator::draw(sf::RenderWindow& window, float totalTime) const {
    sf::RectangleShape linacChassis(sf::Vector2f(m_acceleratorEndX - m_startX + 20.0f, m_plateHeight + 10.0f));
    linacChassis.setPosition(m_startX - 10.0f, m_centerY - (m_plateHeight + 10.0f) / 2);
    linacChassis.setFillColor(sf::Color(16, 20, 28, 220));
    linacChassis.setOutlineColor(sf::Color(35, 45, 60));
    linacChassis.setOutlineThickness(1.0f);
    window.draw(linacChassis);


    float drawX = m_startX;
    for (int i = 0; i < m_numStages; ++i) {
        float currentWidth = m_baseWidth * std::pow(m_growthFactor, i);
        float capLeft = drawX;
        float capRight = capLeft + currentWidth;
        float phaseShift = i * 3.14159f;
        float currentSin = std::sin(m_frequency * totalTime + phaseShift);


        sf::RectangleShape fieldGlow(sf::Vector2f(currentWidth, m_plateHeight));
        fieldGlow.setPosition(capLeft, m_centerY - m_plateHeight / 2);
        sf::Color fieldColor = (currentSin >= 0.0f)
            ? sf::Color(0, 100, 255, static_cast<sf::Uint8>(60 * currentSin))
            : sf::Color(255, 0, 80, static_cast<sf::Uint8>(60 * -currentSin));
        fieldGlow.setFillColor(fieldColor);
        window.draw(fieldGlow);

        sf::Color corePlateColor(38, 45, 56);
        sf::Color borderPlateColor(90, 105, 125);

        sf::RectangleShape topPlate(sf::Vector2f(currentWidth, (m_plateHeight - m_holeSize) / 2));
        topPlate.setPosition(capLeft, m_centerY - m_plateHeight / 2);
        topPlate.setFillColor(corePlateColor);
        topPlate.setOutlineColor(borderPlateColor);
        topPlate.setOutlineThickness(1.0f);

        sf::RectangleShape bottomPlate(sf::Vector2f(currentWidth, (m_plateHeight - m_holeSize) / 2));
        bottomPlate.setPosition(capLeft, m_centerY + m_holeSize / 2);
        bottomPlate.setFillColor(corePlateColor);
        bottomPlate.setOutlineColor(borderPlateColor);
        bottomPlate.setOutlineThickness(1.0f);

        window.draw(topPlate);
        window.draw(bottomPlate);


        sf::RectangleShape powerLedTop(sf::Vector2f(currentWidth, 2.0f));
        powerLedTop.setPosition(capLeft, m_centerY - m_plateHeight / 2 - 1.0f);
        powerLedTop.setFillColor(fieldColor);

        sf::RectangleShape powerLedBottom(sf::Vector2f(currentWidth, 2.0f));
        powerLedBottom.setPosition(capLeft, m_centerY + m_plateHeight / 2 - 1.0f);
        powerLedBottom.setFillColor(fieldColor);

        window.draw(powerLedTop);
        window.draw(powerLedBottom);

        drawX = capRight + m_stageGap;
    }
}