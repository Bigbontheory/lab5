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
#include "SFMLRender.hpp"
#include "../src/PhysicsScale.hpp"

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
    float acceleratorAngle = 0.0f;

    Accelerator accelerator(capIntensity, frequency, 5, 40.0f, 8.0f, 6.0f, 1.60f, 80.0f, 20.0f, centerY);

    float particleMass = 1.0f;
    float particleCharge = 1.0f;
    float startVelocityX = 20.0f;

    Particle particle(PhysicsScale::toPixels(accelerator.getCenter()), sf::Vector2f(0.0f, 0.0f), particleMass, particleCharge);
    bool isLaunched = false;

    std::vector<sf::Vector2f> particleTrail;
    const size_t MAX_TRAIL_SIZE = 25;

    ParticleStream plasmaStream(1000, 600.0f, 1200.0f);

    const int NUM_CHARGES = 12;
    FixedCharge rawCharges[NUM_CHARGES] = {
        FixedCharge(sf::Vector2f(350.0f, 120.0f),  1.5f, 120.0f),
        FixedCharge(sf::Vector2f(420.0f, 520.0f), -1.5f, 120.0f),
        FixedCharge(sf::Vector2f(510.0f, 200.0f),  1.5f, 120.0f),
        FixedCharge(sf::Vector2f(580.0f, 440.0f), -1.5f, 120.0f),
        FixedCharge(sf::Vector2f(660.0f, 100.0f),  1.5f, 120.0f),
        FixedCharge(sf::Vector2f(720.0f, 580.0f), -1.5f, 120.0f),
        FixedCharge(sf::Vector2f(810.0f, 230.0f),  1.5f, 120.0f),
        FixedCharge(sf::Vector2f(890.0f, 390.0f), -1.5f, 120.0f),
        FixedCharge(sf::Vector2f(950.0f, 150.0f),  1.5f, 120.0f),
        FixedCharge(sf::Vector2f(1020.0f, 500.0f), -1.5f, 120.0f),
        FixedCharge(sf::Vector2f(1080.0f, 210.0f),  1.5f, 120.0f),
        FixedCharge(sf::Vector2f(1130.0f, 460.0f), -1.5f, 120.0f)
    };

    MutableArraySequence<FixedCharge> fixedCharges(rawCharges, NUM_CHARGES);

    int selectedChargeIdx = -1;

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

            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                if (!ImGui::GetIO().WantCaptureMouse) {
                    sf::Vector2f mousePos(static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y));
                    int clickedIdx = -1;

                    for (int i = 0; i < fixedCharges.get_size(); ++i) {
                        sf::Vector2f posPx = PhysicsScale::toPixels(fixedCharges.get(i).getPosition());

                        float dx = mousePos.x - posPx.x;
                        float dy = mousePos.y - posPx.y;
                        if (std::sqrt(dx * dx + dy * dy) < 20.0f) {
                            clickedIdx = i;
                            break;
                        }
                    }
                    selectedChargeIdx = clickedIdx;
                }
            }
        }

        float dt = clock.restart().asSeconds();
        if (dt > 0.1f) dt = 0.1f;

        if (isLaunched) {
            totalTime += dt;
        }

        ImGui::SFML::Update(window, sf::seconds(dt));
        plasmaStream.update(dt);

        ImGui::Begin("LINAC: Plasma Stream Controller");
        ImGui::Text("RF Field Settings:");
        ImGui::SliderFloat("Voltage (kV)", &capIntensity, 0.0f, 15000.0f);
        ImGui::SliderFloat("RF Frequency (w)", &frequency, 10.0f, 250.0f);

        if (ImGui::SliderFloat("Accelerator Angle (deg)", &acceleratorAngle, -45.0f, 45.0f, "%.1f deg")) {
            accelerator.setRotation(acceleratorAngle);
        }

        ImGui::Separator();
        ImGui::Text("Lower Stream Settings (1000 Particles):");
        ImGui::SliderFloat("Stream Velocity (m/s)", &plasmaStream.getVelocityRef(), 0.0f, 600.0f);
        ImGui::SliderFloat("Stream Avg Charge", &plasmaStream.getChargeRef(), -10.0f, 10.0f);

        ImGui::Separator();
        ImGui::Text("Ion Properties:");
        ImGui::SliderFloat("Initial Velocity X(m/s)", &startVelocityX, 5.0f, 200.0f);
        ImGui::SliderFloat("Particle Mass(kg)", &particleMass, 0.1f, 10.0f);
        ImGui::SliderFloat("Particle Charge (mq)", &particleCharge, -5.0f, 5.0f, "q: %.1f");

        ImGui::Separator();

        ImGui::TextColored(ImVec4(0.0f, 0.8f, 1.0f, 1.0f), "Tip: Click any target charge on the map\nto edit its properties directly!");

        sf::Vector2f pPosPx = PhysicsScale::toPixels(particle.getPosition());
        if (pPosPx.x >= targetZoneX && pPosPx.x <= targetZoneX + 35.0f) {
            if (pPosPx.y >= targetZoneTopY && pPosPx.y <= targetZoneBottomY) {
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

        if (!isLaunched) {
            if (ImGui::Button("Launch Particle", ImVec2(-1, 30))) {
                float angleRad = acceleratorAngle * (3.14159265f / 180.0f);
                sf::Vector2f launchVelocity(
                    startVelocityX * std::cos(angleRad),
                    startVelocityX * std::sin(angleRad)
                );
                particle = Particle(PhysicsScale::toPixels(accelerator.getCenter()), launchVelocity, particleMass, particleCharge);
                particleTrail.clear();
                isLaunched = true;
            }
        }
        else {
            if (ImGui::Button("Reset / Prepare New", ImVec2(-1, 30))) {
                isLaunched = false;
                particleTrail.clear();
                totalTime = 0.0f;
                levelPassed = false;
            }
        }
        ImGui::End();

        if (selectedChargeIdx != -1) {
            FixedCharge fc = fixedCharges.get(selectedChargeIdx);
            sf::Vector2f posPx = PhysicsScale::toPixels(fc.getPosition());

            ImGui::SetNextWindowPos(ImVec2(posPx.x + 25.0f, posPx.y - 60.0f), ImGuiCond_Always);
            ImGui::SetNextWindowSize(ImVec2(240.0f, 110.0f), ImGuiCond_Always);

            std::string title = "Target #" + std::to_string(selectedChargeIdx + 1) + " Properties";

            ImGuiWindowFlags flags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove;

            if (ImGui::Begin(title.c_str(), nullptr, flags)) {
                bool isChanged = false;

                float qVal = fc.getCharge();
                float qUi = qVal * 1e6f;
                if (ImGui::SliderFloat("Charge", &qUi, -150.0f, 150.0f, "%.1f mCl")) {
                    fc.setCharge(qUi);
                    isChanged = true;
                }

                float mVal = fc.getMass();
                float mUi = mVal / 1e9f;
                if (ImGui::SliderFloat("Mass", &mUi, 0.0f, 500.0f, "M: %.1f")) {
                    fc.setMass(mUi);
                    isChanged = true;
                }

                ImGui::Spacing();
                if (ImGui::Button("Close Panel", ImVec2(-1, 20))) {
                    selectedChargeIdx = -1;
                }

                if (isChanged) {
                    fixedCharges.set(selectedChargeIdx, fc);
                }
            }
            ImGui::End();
        }

        if (isLaunched) {
            sf::Vector2f totalForce(0.0f, 0.0f);
            sf::Vector2f currentPos = particle.getPosition();
            sf::Vector2f currentVel = particle.getVelocity();

            particleTrail.push_back(PhysicsScale::toPixels(currentPos));
            if (particleTrail.size() > MAX_TRAIL_SIZE) {
                particleTrail.erase(particleTrail.begin());
            }

            sf::Vector2f fieldForce(0.0f, 0.0f);
            bool insideAnyCapacitor = accelerator.updateFieldForce(currentPos, particle.getCharge(), totalTime, fieldForce);

            if (insideAnyCapacitor) {
                totalForce = fieldForce;
            }
            else {
                totalForce = sf::Vector2f(0.0f, 0.0f);

                for (int i = 0; i < fixedCharges.get_size(); ++i) {
                    const FixedCharge& fc = fixedCharges.get(i);
                    sf::Vector2f targetPos = fc.getPosition();
                    sf::Vector2f diff = currentPos - targetPos;
                    float dSq = diff.x * diff.x + diff.y * diff.y;

                    if (dSq < 0.0225f) {
                        float dist = std::sqrt(dSq);
                        sf::Vector2f normal = diff / (dist > 0.0f ? dist : 1.0f);
                        totalForce += normal * 1000.0f;
                    }

                    totalForce += Physics::calculateCoulombForce(currentPos, particle.getCharge(), targetPos, fc.getCharge());
                    totalForce += Physics::calculateGravityForce(currentPos, particleMass, targetPos, fc.getMass());
                }

                float B = plasmaStream.calculateMagneticFieldAt(currentPos);
                if (!std::isnan(B)) {
                    sf::Vector2f lForce = Physics::calculateLorentzForce(particle.getCharge(), B, currentVel);
                    totalForce += lForce * 1000000.0f;
                }
            }

            particle.update(totalForce, dt);
        }
        else {
            particle = Particle(PhysicsScale::toPixels(accelerator.getCenter()), sf::Vector2f(0.0f, 0.0f), particleMass, particleCharge);
            particleTrail.clear();
        }

        sf::Vector2f screenPos = PhysicsScale::toPixels(particle.getPosition());
        if (screenPos.y < -100.0f || screenPos.y > 800.0f || screenPos.x > 1250.0f) {
            if (!levelPassed) {
                isLaunched = false;
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

        AcceleratorRenderer::draw(window, accelerator, totalTime);
        ParticleStreamRenderer::draw(window, plasmaStream);

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

            sf::Vector2f posPx = PhysicsScale::toPixels(fc.getPosition());

            if (i == selectedChargeIdx) {
                sf::CircleShape selectionRing(18.0f);
                selectionRing.setOrigin(18.0f, 18.0f);
                selectionRing.setPosition(posPx);
                selectionRing.setFillColor(sf::Color::Transparent);
                selectionRing.setOutlineColor(sf::Color(0, 255, 150, 220));
                selectionRing.setOutlineThickness(2.0f);
                window.draw(selectionRing);
            }

            if (font.getInfo().family != "") {
                sf::Text text;
                text.setFont(font);
                text.setString("#" + std::to_string(i + 1));
                text.setCharacterSize(14);
                text.setStyle(sf::Text::Bold);

                if (i == selectedChargeIdx) text.setFillColor(sf::Color(0, 255, 150, 255));
                else text.setFillColor(sf::Color(255, 255, 255, 160));

                text.setPosition(posPx.x + 14.0f, posPx.y - 22.0f);
                window.draw(text);
            }
        }

        ParticleR::draw(window, particle);

        ImGui::SFML::Render(window);
        window.display();
    }

    ImGui::SFML::Shutdown();
    return 0;
}