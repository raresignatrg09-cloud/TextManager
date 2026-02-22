#include "TextManager.h"
#include <algorithm>
#include <stdexcept>

static uint8_t lerp(uint8_t a, uint8_t b, float t)
{
    return static_cast<uint8_t>(a + (b - a) * t);
}

// ================= FONT =================

bool TextManager::loadFont(std::string_view id, const std::filesystem::path& path)
{
    sf::Font font;
    if (!font.openFromFile(path)) 
        return false;
    
	fonts[std::string(id)] = std::move(font);
    return true;
}

void TextManager::clearFonts() 
{ 
    fonts.clear(); 
}

// ================= TEXT =================

void TextManager::createText(std::string_view id,
    std::string_view fontID,
    std::string_view content,
    unsigned int size,
    sf::Vector2f position)
{
    auto fontIt = fonts.find(std::string(fontID));
    if (fontIt == fonts.end())
        throw std::runtime_error("Font ID not found: " + std::string(fontID));

    texts.emplace(std::string(id),
        ManagedText(fontIt->second, content, size, position));
}

void TextManager::removeText(std::string_view id)
{ 
    texts.erase(std::string(id)); 
}

void TextManager::clearTexts() 
{ 
    texts.clear(); 
}

bool TextManager::contains(std::string_view id) const
{
    return texts.find(std::string(id)) != texts.end();
}

// ================= PROPERTIES =================

void TextManager::setString(std::string_view id, std::string_view content)
{
    auto& mt = texts.at(std::string(id));
    mt.text.setString(std::string(content));

    // Keep text centered properly
    updateAlignment(mt);

    // Update shadow if exists
    if (mt.shadow)
        mt.shadow->setString(std::string(content));
}

void TextManager::setPosition(std::string_view id, sf::Vector2f position)
{
    auto& mt = texts.at(std::string(id));
    mt.text.setPosition(position);

    if (mt.shadow)
        mt.shadow->setPosition(position + mt.shadowOffset);
}

void TextManager::setColor(std::string_view id, sf::Color color)
{
    get(id).setFillColor(color);
}

void TextManager::setCharacterSize(std::string_view id, unsigned int size)
{
    auto& mt = texts.at(std::string(id));
    mt.text.setCharacterSize(size);
    updateAlignment(mt);
}

void TextManager::setStyle(std::string_view id, sf::Text::Style style)
{
    get(id).setStyle(style);
}

void TextManager::setOutline(std::string_view id, float thickness, sf::Color color)
{
    auto& mt = get(id);
    mt.setOutlineThickness(thickness);
    mt.setOutlineColor(color);
}

void TextManager::setAlignment(std::string_view id, Alignment alignment)
{
    auto& mt = texts.at(std::string(id));
    mt.alignment = alignment;
    updateAlignment(mt);
}

void TextManager::setVisible(std::string_view id, bool visible)
{
    texts.at(std::string(id)).visible = visible;
}

void TextManager::setShadow(std::string_view id, sf::Color color, sf::Vector2f offset)
{
    auto& mt = texts.at(std::string(id));

    mt.shadow.emplace(mt.text);
    mt.shadow->setFillColor(color);

    mt.shadowOffset = offset;
    mt.shadow->setPosition(mt.text.getPosition() + offset);
}

// ================= ANIMATION =================

void TextManager::animateFade(std::string_view id,
    sf::Color start,
    sf::Color end,
    sf::Time duration,
    bool loop)
{
    auto& mt = texts.at(std::string(id));
    mt.animation.type = AnimationType::Fade;
    mt.animation.startColor = start;
    mt.animation.endColor = end;
    mt.animation.duration = duration;
    mt.animation.elapsed = sf::Time::Zero;
    mt.animation.loop = loop;

    mt.text.setFillColor(start);
}

void TextManager::animateScale(std::string_view id,
    float start,
    float end,
    sf::Time duration,
    bool loop)
{
    auto& mt = texts.at(std::string(id));
    mt.animation.type = AnimationType::Scale;
    mt.animation.startScale = start;
    mt.animation.endScale = end;
    mt.animation.duration = duration;
    mt.animation.elapsed = sf::Time::Zero;
    mt.animation.loop = loop;

    mt.text.setScale({ start, start });
}

void TextManager::update(sf::Time dt)
{
    for (auto& [id, mt] : texts)
    {
        if (mt.animation.type == AnimationType::None)
            continue;

        mt.animation.elapsed += dt;

        float t = std::min(
            mt.animation.elapsed.asSeconds() /
            mt.animation.duration.asSeconds(),
            1.f
        );

        if (mt.animation.type == AnimationType::Fade)
        {
            sf::Color c(
                lerp(mt.animation.startColor.r, mt.animation.endColor.r, t),
                lerp(mt.animation.startColor.g, mt.animation.endColor.g, t),
                lerp(mt.animation.startColor.b, mt.animation.endColor.b, t),
                lerp(mt.animation.startColor.a, mt.animation.endColor.a, t)
            );

            mt.text.setFillColor(c);
            if (mt.shadow)
                mt.shadow->setFillColor(c);
        }
        else if (mt.animation.type == AnimationType::Scale)
        {
            float scale =
                mt.animation.startScale +
                (mt.animation.endScale - mt.animation.startScale) * t;

            mt.text.setScale({ scale, scale });
            if (mt.shadow)
                mt.shadow->setScale({ scale, scale });
        }

        if (t >= 1.f)
        {
            if (mt.animation.loop)
                mt.animation.elapsed = sf::Time::Zero;
            else
                mt.animation.type = AnimationType::None;
        }
    }
}

// ================= DRAW =================

void TextManager::draw(sf::RenderTarget& target) const
{
    for (const auto& [id, mt] : texts)
    {
        if (!mt.visible)
            continue;

        if (mt.shadow)
            target.draw(*mt.shadow);

        target.draw(mt.text);
    }
}

// ================= ACCESS =================

sf::Text& TextManager::get(std::string_view id)
{
    return texts.at(std::string(id)).text;
}

const sf::Text& TextManager::get(std::string_view id) const
{
    return texts.at(std::string(id)).text;
}

// ================= PRIVATE =================

void TextManager::updateAlignment(ManagedText& mt)
{
    auto bounds = mt.text.getLocalBounds();

    switch (mt.alignment)
    {
    case Alignment::Left:
        mt.text.setOrigin({ 0.f, bounds.position.y });
        break;

    case Alignment::Center:
        mt.text.setOrigin({
            bounds.position.x + bounds.size.x / 2.f,
            bounds.position.y + bounds.size.y / 2.f
            });
        break;

    case Alignment::Right:
        mt.text.setOrigin({
            bounds.position.x + bounds.size.x,
            bounds.position.y
            });
        break;
    }
}
