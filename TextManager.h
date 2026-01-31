#pragma once
#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <vector>
#include <string>
#include <functional>

struct TextAnimation
{
    enum class Type { None, Fade, Scale };
    Type type = Type::None;
    float duration = 1.f;
    float elapsed = 0.f;
    sf::Color startColor;
    sf::Color endColor;
    float startScale = 1.f;
    float endScale = 1.f;
    bool loop = false;
};

class TextManager
{
public:
    TextManager() = default;

    // ------------------------
    // Font Management
    // ------------------------
    bool loadFont(const std::string& path);
    void clearFonts();

    // ------------------------
    // Text Management
    // ------------------------
    void addText(const std::string& id, const std::string& content,
        unsigned int size, const sf::Vector2f& position);
    void removeText(const std::string& id);
    void clearTexts();

    // ------------------------
    // Text Properties
    // ------------------------
    void setText(const std::string& id, const std::string& content);
    void setPosition(const std::string& id, const sf::Vector2f& position);
    void setColor(const std::string& id, const sf::Color& color);
    void setOutline(const std::string& id, float thickness, const sf::Color& color);
    void setCharacterSize(const std::string& id, unsigned int size);
    void setStyle(const std::string& id, sf::Text::Style style);
    void centerText(const std::string& id, float x, float y);
    void setShadow(const std::string& id, const sf::Color& color, const sf::Vector2f& offset);
	void setVisible(const std::string& id, bool visible);
	void setAllVisible(bool visible);
    sf::Text* get(const std::string& id);

    // ------------------------
    // Animation
    // ------------------------
    void animateTextFade(const std::string& id, sf::Color start, sf::Color end,
        float duration, bool loop = false);
    void animateTextScale(const std::string& id, float start, float end,
        float duration, bool loop = false);
    void updateAnimations(float dt);

    // ------------------------
    // Drawing
    // ------------------------
    void draw(sf::RenderTarget& target);

private:
    struct ManagedText
    {
        sf::Text text;
        sf::Text shadow;
        bool hasShadow = false;
		bool visible = true;
        TextAnimation animation;
    };

    std::vector<sf::Font> fonts;
    std::unordered_map<std::string, ManagedText> texts;
};
