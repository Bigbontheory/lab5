#pragma once

#include <cmath>
#include <SFML/Graphics.hpp>

class ParticleStreamRenderer {
public:
    static void draw(sf::RenderWindow& window, const ParticleStream& stream) {
        int size = stream.get_size();
        if (size == 0) return;

        float charge = stream.getCharge();

        for (int i = 0; i < size; ++i) {
            const StreamParticle& p = stream.getParticle(i);

            sf::CircleShape dot(3.0f);
            dot.setOrigin(3.0f, 3.0f);
            dot.setPosition(p.position);

            if (charge > 0.0f) {
                dot.setFillColor(sf::Color(0, 255, 150, 220));
            }
            else if (charge < 0.0f) {
                dot.setFillColor(sf::Color(255, 150, 0, 220));
            }
            else {
                dot.setFillColor(sf::Color(150, 150, 150, 150));
            }
            window.draw(dot);
        }
    }
};

#pragma once
#include <SFML/Graphics.hpp>
#include "../src/Particle.hpp"


class ParticleR {
public:
    ParticleR() = delete;

    static void draw(sf::RenderTarget& target, const Particle& particle) {
        float charge = particle.getCharge();
        sf::Vector2f position = particle.getPosition();

        float radius = 6.0f;
        sf::CircleShape shape(radius);
        shape.setOrigin(radius, radius);
        shape.setPosition(position);

        if (charge > 0.0f) {
            shape.setFillColor(sf::Color::Red);
        }
        else if (charge < 0.0f) {
            shape.setFillColor(sf::Color::Blue);
        }
        else {
            shape.setFillColor(sf::Color::White);
        }

        target.draw(shape);
    }
};


class AcceleratorRenderer {
public:
    static void draw(sf::RenderWindow& window, const Accelerator& accelerator, float totalTime) {
        float startX = accelerator.getStartX();
        float endX = accelerator.getEndX();
        float centerY = accelerator.getCenterY();
        float plateHeight = accelerator.getPlateHeight();
        float holeSize = accelerator.getHoleSize();
        float baseWidth = accelerator.getBaseWidth();
        float growthFactor = accelerator.getGrowthFactor();
        float stageGap = accelerator.getStageGap();
        int numStages = accelerator.getNumStages();

        float frequency = accelerator.getFrequency();
        float capIntensity = accelerator.getCapIntensity();

        // 1. Отрисовка шасси LINAC
        sf::RectangleShape linacChassis(sf::Vector2f(endX - startX + 20.0f, plateHeight + 10.0f));
        linacChassis.setPosition(startX - 10.0f, centerY - (plateHeight + 10.0f) / 2);
        linacChassis.setFillColor(sf::Color(16, 20, 28, 220));
        linacChassis.setOutlineColor(sf::Color(35, 45, 60));
        linacChassis.setOutlineThickness(1.0f);
        window.draw(linacChassis);

        // 2. Отрисовка каскадов (пластины и неоновые зазоры)
        float drawX = startX;
        for (int i = 0; i < numStages; ++i) {
            float currentWidth = baseWidth * std::pow(growthFactor, i);
            float capLeft = drawX;
            float capRight = capLeft + currentWidth;
            float phaseShift = i * 3.14159f;
            float currentSin = std::sin(frequency * totalTime + phaseShift);

            // Свечение ВЧ-поля
            sf::RectangleShape fieldGlow(sf::Vector2f(currentWidth, plateHeight));
            fieldGlow.setPosition(capLeft, centerY - plateHeight / 2);
            sf::Color fieldColor = (currentSin >= 0.0f)
                ? sf::Color(0, 100, 255, static_cast<sf::Uint8>(60 * currentSin))
                : sf::Color(255, 0, 80, static_cast<sf::Uint8>(60 * -currentSin));
            fieldGlow.setFillColor(fieldColor);
            window.draw(fieldGlow);

            sf::Color corePlateColor(38, 45, 56);
            sf::Color borderPlateColor(90, 105, 125);

            // Верхняя пластина
            sf::RectangleShape topPlate(sf::Vector2f(currentWidth, (plateHeight - holeSize) / 2));
            topPlate.setPosition(capLeft, centerY - plateHeight / 2);
            topPlate.setFillColor(corePlateColor);
            topPlate.setOutlineColor(borderPlateColor);
            topPlate.setOutlineThickness(1.0f);

            // Нижняя пластина
            sf::RectangleShape bottomPlate(sf::Vector2f(currentWidth, (plateHeight - holeSize) / 2));
            bottomPlate.setPosition(capLeft, centerY + holeSize / 2);
            bottomPlate.setFillColor(corePlateColor);
            bottomPlate.setOutlineColor(borderPlateColor);
            bottomPlate.setOutlineThickness(1.0f);

            window.draw(topPlate);
            window.draw(bottomPlate);

            // Индикаторы (LED) на границах
            sf::RectangleShape powerLedTop(sf::Vector2f(currentWidth, 2.0f));
            powerLedTop.setPosition(capLeft, centerY - plateHeight / 2 - 1.0f);
            powerLedTop.setFillColor(fieldColor);

            sf::RectangleShape powerLedBottom(sf::Vector2f(currentWidth, 2.0f));
            powerLedBottom.setPosition(capLeft, centerY + plateHeight / 2 - 1.0f);
            powerLedBottom.setFillColor(fieldColor);

            window.draw(powerLedTop);
            window.draw(powerLedBottom);

            drawX = capRight + stageGap;
        }
    }
};

class FixedChargeR {
public:
    static void draw(sf::RenderTarget& target, const FixedCharge& fc) {
        float charge = fc.getCharge();
        sf::Vector2f position = fc.getPosition();

        if (std::abs(charge) > 0.01f) {
            float glowRadius = 20.0f;
            sf::CircleShape glow(glowRadius);
            glow.setOrigin(glowRadius, glowRadius);
            glow.setPosition(position);

            if (charge > 0.0f) {
                glow.setFillColor(sf::Color(255, 60, 60, 45));
            }
            else {
                glow.setFillColor(sf::Color(60, 60, 255, 45));
            }
            target.draw(glow);
        }
        float radius = 6.0f;
        sf::CircleShape shape(radius);
        shape.setOrigin(radius, radius);
        shape.setPosition(position);

        if (charge > 0.0f) {
            shape.setFillColor(sf::Color(255, 50, 50));
            shape.setOutlineColor(sf::Color(255, 150, 150));
            shape.setOutlineThickness(2.0f);
        }
        else if (charge < 0.0f) {
            shape.setFillColor(sf::Color(50, 50, 255));
            shape.setOutlineColor(sf::Color(150, 150, 255));
            shape.setOutlineThickness(2.0f);
        }
        else {
            shape.setFillColor(sf::Color(120, 120, 120));
            shape.setOutlineThickness(0.0f);
        }

        target.draw(shape);
    }
};