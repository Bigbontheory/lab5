#include <SFML/Graphics.hpp>
#include <imgui.h>
#include <imgui-SFML.h>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>

#include "../src/Particle.hpp"
#include "../src/FixedCharge.hpp"
#include "../src/Physics.hpp"
#include "../LAB2/mutable_array_sequence.hpp" 
#include "../src/ParticleStream.hpp"
#include "../src/Accelerator.hpp" 
#include "../src/GuiTheme.hpp"

#include "FixedChargeRenderer.hpp"
int main() {
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    sf::RenderWindow window(sf::VideoMode(1200, 700), "LINAC: Particle Stream Sandbox", sf::Style::Default, settings);
    window.setFramerateLimit(60);

    if (!ImGui::SFML::Init(window)) {
        return -1;
    }
    GuiTheme::ApplySciFi();
    sf::Font font;
    if (!font.loadFromFile("C:/Windows/Fonts/arial.ttf")) {
        font.loadFromFile("arial.ttf");
    }

    float capIntensity = 5000.0f;
    float frequency = 54.0f;
    float centerY = 280.0f;

    Accelerator accelerator(capIntensity, frequency, 5, 40.0f, 8.0f, 6.0f, 1.60f, 80.0f, 20.0f, centerY);

    float particleMass = 1.0f;
    float particleCharge = 1.0f;
    float startVelocityX = 20.0f;

    Particle particle(sf::Vector2f(20.0f, centerY), sf::Vector2f(startVelocityX, 0.0f), particleMass, particleCharge);

    std::vector<sf::Vector2f> particleTrail;
    const size_t MAX_TRAIL_SIZE = 25;

    ParticleStream plasmaStream(100, 600.0f, 1200.0f);

    const int NUM_CHARGES = 12;
    FixedCharge rawCharges[NUM_CHARGES] = {
        FixedCharge(sf::Vector2f(350.0f, 120.0f),  45.0f, 120.0f),
        FixedCharge(sf::Vector2f(420.0f, 520.0f), -45.0f, 120.0f),
        FixedCharge(sf::Vector2f(510.0f, 200.0f),  45.0f, 120.0f),
        FixedCharge(sf::Vector2f(580.0f, 440.0f), -45.0f, 120.0f),
        FixedCharge(sf::Vector2f(660.0f, 100.0f),  45.0f, 120.0f),
        FixedCharge(sf::Vector2f(720.0f, 580.0f), -45.0f, 120.0f),
        FixedCharge(sf::Vector2f(810.0f, 230.0f),  45.0f, 120.0f),
        FixedCharge(sf::Vector2f(890.0f, 390.0f), -45.0f, 120.0f),
        FixedCharge(sf::Vector2f(950.0f, 150.0f),  45.0f, 120.0f),
        FixedCharge(sf::Vector2f(1020.0f, 500.0f), -45.0f, 120.0f),
        FixedCharge(sf::Vector2f(1080.0f, 210.0f),  45.0f, 120.0f),
        FixedCharge(sf::Vector2f(1130.0f, 460.0f), -45.0f, 120.0f)
    };

    MutableArraySequence<FixedCharge> fixedCharges(rawCharges, NUM_CHARGES);

    const float targetZoneX = 1160.0f;
    const float targetZoneTopY = 60.0f;
    const float targetZoneBottomY = 140.0f;

    sf::Clock clock;
    float totalTime = 0.0f;
    bool levelPassed = false;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            ImGui::SFML::ProcessEvent(window, event);
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        float dt = clock.restart().asSeconds();
        if (dt > 0.1f) dt = 0.1f;
        totalTime += dt;
        ImGui::SFML::Update(window, sf::seconds(dt));

        plasmaStream.update(dt);

        ImGui::Begin("LINAC: Plasma Stream Controller");
        ImGui::Text("RF Field Settings:");
        ImGui::SliderFloat("Voltage (E)", &capIntensity, 0.0f, 15000.0f);
        ImGui::SliderFloat("RF Frequency (w)", &frequency, 10.0f, 250.0f);

        ImGui::Separator();
        ImGui::Text("Lower Stream Settings (100 Particles):");
        ImGui::SliderFloat("Stream Velocity", &plasmaStream.getVelocityRef(), 0.0f, 600.0f);
        ImGui::SliderFloat("Stream Avg Charge", &plasmaStream.getChargeRef(), -10.0f, 10.0f);

        ImGui::Separator();
        ImGui::Text("Ion Properties:");
        ImGui::SliderFloat("Initial Velocity X", &startVelocityX, 5.0f, 200.0f);
        ImGui::SliderFloat("Particle Mass", &particleMass, 0.1f, 10.0f);
        ImGui::SliderFloat("Particle Charge (q)", &particleCharge, -5.0f, 5.0f, "q: %.1f");

        ImGui::Separator();
        ImGui::Text("Static Map Obstacles (12 Targets):");

        if (ImGui::TreeNode("Tune Static Field Weights")) {
            ImGui::Spacing();
            ImGui::Columns(3, "ObstaclesColumns", true);
            ImGui::SetColumnWidth(0, 110.0f);
            ImGui::SetColumnWidth(1, 180.0f);
            ImGui::SetColumnWidth(2, 180.0f);

            ImGui::Text("Obstacle"); ImGui::NextColumn();
            ImGui::Text("Charge (Q)"); ImGui::NextColumn();
            ImGui::Text("Mass (M)"); ImGui::NextColumn();
            ImGui::Separator();

            for (int i = 0; i < fixedCharges.get_size(); ++i) {
                FixedCharge& fc = fixedCharges.get(i);
                ImGui::PushID(i);

                ImGui::Text("Obstacle #%d", i + 1);
                ImGui::NextColumn();

                float qVal = fc.getCharge();
                ImGui::PushItemWidth(-1);
                if (ImGui::SliderFloat("##Q", &qVal, -150.0f, 150.0f, "Q: %.1f")) {
                    fc.setCharge(qVal);
                }
                ImGui::PopItemWidth();
                ImGui::NextColumn();

                float mVal = fc.getMass();
                ImGui::PushItemWidth(-1);
                if (ImGui::SliderFloat("##M", &mVal, 0.0f, 500.0f, "M: %.1f")) {
                    fc.setMass(mVal);
                }
                ImGui::PopItemWidth();
                ImGui::NextColumn();

                ImGui::PopID();
            }
            ImGui::Columns(1);
            ImGui::TreePop();
        }

        sf::Vector2f pPos = particle.getPosition();
        if (pPos.x >= targetZoneX && pPos.x <= targetZoneX + 35.0f) {
            if (pPos.y >= targetZoneTopY && pPos.y <= targetZoneBottomY) {
                levelPassed = true;
            }
        }

        ImGui::Separator();
        if (levelPassed) {
            ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "=== TARGET ZONE HIT! SUCCESS!!! ===");
        }
        else {
            ImGui::Text("Status: Steady Map Trajectory...");
        }

        ImGui::Spacing();
        if (ImGui::Button("Launch / Reset", ImVec2(-1, 30))) {
            particle = Particle(sf::Vector2f(20.0f, centerY), sf::Vector2f(startVelocityX, 0.0f), particleMass, particleCharge);
            particleTrail.clear();
            totalTime = 0.0f;
            levelPassed = false;
        }
        ImGui::End();

        sf::Vector2f totalForce(0.0f, 0.0f);
        sf::Vector2f currentPos = particle.getPosition();
        sf::Vector2f currentVel = particle.getVelocity();
        particleTrail.push_back(currentPos);
        if (particleTrail.size() > MAX_TRAIL_SIZE) {
            particleTrail.erase(particleTrail.begin());
        }

        sf::Vector2f fieldForce(0.0f, 0.0f);
        bool insideAnyCapacitor = accelerator.updateFieldForce(currentPos, particle.getCharge(), totalTime, fieldForce);

        if (insideAnyCapacitor) {
            totalForce = fieldForce;
        }
        else {
            if (currentPos.x > accelerator.getEndX()) {
                for (int i = 0; i < fixedCharges.get_size(); ++i) {
                    const FixedCharge& fc = fixedCharges.get(i);
                    sf::Vector2f targetPos = fc.getPosition();
                    sf::Vector2f diff = currentPos - targetPos;
                    float dSq = diff.x * diff.x + diff.y * diff.y;

                    if (dSq < 225.0f) {
                        float dist = std::sqrt(dSq);
                        sf::Vector2f normal = diff / (dist > 0.0f ? dist : 1.0f);
                        totalForce += normal * 8000.0f;
                    }
                    totalForce += Physics::calculateCoulombForce(currentPos, particle.getCharge(), targetPos, fc.getCharge());
                    totalForce += Physics::calculateGravityForce(currentPos, particleMass, targetPos, fc.getMass());
                }
            }
            float B = plasmaStream.calculateMagneticFieldAt(currentPos);
            if (!std::isnan(B)) {
                sf::Vector2f lorentzForce(particle.getCharge() * currentVel.y * B, -particle.getCharge() * currentVel.x * B);
                totalForce += lorentzForce;
            }
        }

        particle.update(totalForce, dt);

        if (particle.getPosition().y < -100.0f || particle.getPosition().y > 800.0f || particle.getPosition().x > 1250.0f) {
            if (!levelPassed) {
                particle = Particle(sf::Vector2f(20.0f, centerY), sf::Vector2f(startVelocityX, 0.0f), particleMass, particleCharge);
                particleTrail.clear();
                totalTime = 0.0f;
            }
        }

        window.clear(sf::Color(8, 10, 15));

        sf::Color gridColor(20, 26, 38);
        for (int x = 0; x < 1200; x += 40) {
            sf::Vertex line[] = { sf::Vertex(sf::Vector2f(static_cast<float>(x), 0.0f), gridColor), sf::Vertex(sf::Vector2f(static_cast<float>(x), 700.0f), gridColor) };
            window.draw(line, 2, sf::Lines);
        }
        for (int y = 0; y < 700; y += 40) {
            sf::Vertex line[] = { sf::Vertex(sf::Vector2f(0.0f, static_cast<float>(y)), gridColor), sf::Vertex(sf::Vector2f(1200.0f, static_cast<float>(y)), gridColor) };
            window.draw(line, 2, sf::Lines);
        }

        float currentB = plasmaStream.calculateMagneticFieldAt(particle.getPosition());
        if (!std::isnan(currentB)) {
            sf::RectangleShape magGlow(sf::Vector2f(1200.0f, 700.0f));
            if (currentB > 0.0f) magGlow.setFillColor(sf::Color(0, 255, 255, static_cast<sf::Uint8>(std::min(currentB * 100.0f, 30.0f))));
            else magGlow.setFillColor(sf::Color(255, 0, 255, static_cast<sf::Uint8>(std::min(-currentB * 100.0f, 30.0f))));
            window.draw(magGlow);
        }

        sf::RectangleShape targetVisual(sf::Vector2f(20.0f, targetZoneBottomY - targetZoneTopY));
        targetVisual.setPosition(targetZoneX, targetZoneTopY);
        if (levelPassed) {
            targetVisual.setFillColor(sf::Color(0, 255, 150, 60));
            targetVisual.setOutlineColor(sf::Color(0, 255, 150, 200));
        }
        else {
            targetVisual.setFillColor(sf::Color(0, 180, 255, 25));
            targetVisual.setOutlineColor(sf::Color(0, 180, 255, 120));
        }
        targetVisual.setOutlineThickness(2.0f);
        window.draw(targetVisual);

        accelerator.draw(window, totalTime);
        plasmaStream.draw(window);

        for (size_t i = 0; i < particleTrail.size(); ++i) {
            float alphaFactor = static_cast<float>(i) / particleTrail.size();
            float radius = 1.0f + alphaFactor * 4.0f;

            sf::CircleShape trailDot(radius);
            trailDot.setOrigin(radius, radius);
            trailDot.setPosition(particleTrail[i]);
            trailDot.setFillColor(sf::Color(0, 255, 200, static_cast<sf::Uint8>(180 * alphaFactor)));
            window.draw(trailDot);
        }

        for (int i = 0; i < fixedCharges.get_size(); ++i) {
            const FixedCharge& fc = fixedCharges.get(i);

            FixedChargeR::draw(window, fc);

            if (font.getInfo().family != "") {
                sf::Text text;
                text.setFont(font);
                text.setString("#" + std::to_string(i + 1));
                text.setCharacterSize(14);
                text.setStyle(sf::Text::Bold);
                text.setFillColor(sf::Color(255, 255, 255, 160));
                sf::Vector2f pos = fc.getPosition();
                text.setPosition(pos.x + 14.0f, pos.y - 22.0f);
                window.draw(text);
            }
        }

        particle.draw(window);

        ImGui::SFML::Render(window);
        window.display();
    }

    ImGui::SFML::Shutdown();
    return 0;
}