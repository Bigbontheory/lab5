#pragma once

#include <SFML/Graphics.hpp>
#include <cmath>
#include "../LAB2/mutable_array_sequence.hpp"

class Accelerator {
private:
    // Конфигурационные параметры геометрии LINAC
    int m_numStages;
    float m_startX;
    float m_baseWidth;
    float m_stageGap;
    float m_growthFactor;
    float m_plateHeight;
    float m_holeSize;
    float m_centerY;

    // Ссылки на динамические параметры поля (управляются через GUI)
    float& m_capIntensity;
    float& m_frequency;

    // Вычисляемая координата конца ускорителя для физики
    float m_acceleratorEndX;

    // Внутренний метод для предварительного расчета геометрии
    void calculateEndPosition();

public:
    // Конструктор принимает изменяемые параметры поля по ссылке, чтобы main и GUI работали со сквозными данными
    Accelerator(float& capIntensity, float& frequency,
        int numStages = 5, float startX = 40.0f, float baseWidth = 8.0f,
        float stageGap = 6.0f, float growthFactor = 1.60f, float plateHeight = 80.0f,
        float holeSize = 20.0f, float centerY = 280.0f);

    ~Accelerator() = default;

    // Геттеры для физического движка
    float getStartX() const { return m_startX; }
    float getEndX() const { return m_acceleratorEndX; }
    int getNumStages() const { return m_numStages; }
    float getStageGap() const { return m_stageGap; }

    // Расчет силы и обновление состояния ВЧ-поля внутри ускорителя
    bool updateFieldForce(const sf::Vector2f& particlePos, float particleCharge, float totalTime, sf::Vector2f& outForce);

    // Отрисовка шасси, неонового зазора и металлических пластин
    void draw(sf::RenderWindow& window, float totalTime) const;
};