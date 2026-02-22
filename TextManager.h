#pragma once
#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <vector>
#include <string>
#include <functional>
#include <filesystem>
#include <string_view>
#include <optional>

class TextManager
{
public:
	enum class AnimationType { None, Fade, Scale };
	enum class Alignment { Left, Center, Right };

    TextManager() = default;

    // ---------------- Font ----------------
    bool loadFont(std::string_view id, const std::filesystem::path& path);
    void clearFonts();

    // ---------------- Text ----------------
    void createText(std::string_view id,
        std::string_view fontID,
        std::string_view content,
        unsigned int size,
        sf::Vector2f position);

    void removeText(std::string_view id);
    void clearTexts();

    bool contains(std::string_view id) const;

    // ---------------- Properties ----------------
    void setString(std::string_view id, std::string_view content);
    void setPosition(std::string_view id, sf::Vector2f position);
    void setColor(std::string_view id, sf::Color color);
    void setCharacterSize(std::string_view id, unsigned int size);
    void setStyle(std::string_view id, sf::Text::Style style);
    void setOutline(std::string_view id, float thickness, sf::Color color);
    void setAlignment(std::string_view id, Alignment alignment);
    void setVisible(std::string_view id, bool visible);

    void setShadow(std::string_view id, sf::Color color, sf::Vector2f offset);

    // ---------------- Animation ----------------
    void animateFade(std::string_view id,
        sf::Color start,
        sf::Color end,
        sf::Time duration,
        bool loop = false);

    void animateScale(std::string_view id,
        float start,
        float end,
        sf::Time duration,
        bool loop = false);

    void update(sf::Time dt);

    // ---------------- Draw ----------------
    void draw(sf::RenderTarget& target) const;

    // Safe access
    sf::Text& get(std::string_view id);
    const sf::Text& get(std::string_view id) const;

private:
    struct Animation
    {
		AnimationType type = AnimationType::None;
		sf::Time elapsed = sf::Time::Zero;
        sf::Time duration = sf::Time::Zero;
        bool loop = false;
        
        sf::Color startColor;
        sf::Color endColor;
        
        float startScale = 1.f;
		float endScale = 1.f;
    };

    struct ManagedText
    {
        sf::Text text;

        std::optional<sf::Text> shadow;
        sf::Vector2f shadowOffset{ 0.f, 0.f };

        Animation animation;
        bool visible = true;
        Alignment alignment = Alignment::Left;

        ManagedText(const sf::Font& font,
            std::string_view content,
            unsigned int size,
            sf::Vector2f position)
            : text(font, std::string(content), size)
        {
            text.setPosition(position);
            text.setFillColor(sf::Color::White);
        }
    };
	std::unordered_map<std::string, sf::Font> fonts;
	std::unordered_map<std::string, ManagedText> texts;

private:
	void updateAlignment(ManagedText& mt);   
};
