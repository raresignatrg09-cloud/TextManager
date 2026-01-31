#include "TextManager.h"
#include <algorithm>

// ------------------------
// Font Management
// ------------------------
bool TextManager::loadFont(const std::string& path)
{
    sf::Font font;
    if (!font.loadFromFile(path)) return false;
    fonts.insert(fonts.begin(), font); // main font first
    return true;
}

void TextManager::clearFonts() { fonts.clear(); }

// ------------------------
// Text Management
// ------------------------
void TextManager::addText(const std::string& id, const std::string& content,
    unsigned int size, const sf::Vector2f& position)
{
    if (fonts.empty()) return;

    ManagedText mt;
    mt.text.setFont(fonts[0]);
    mt.text.setCharacterSize(size);
    mt.text.setPosition(position);
    mt.text.setFillColor(sf::Color::White);
    mt.text.setString(sf::String::fromUtf8(content.begin(), content.end()));
    mt.text.setStyle(sf::Text::Regular);

    texts[id] = mt;
}

void TextManager::removeText(const std::string& id) { texts.erase(id); }
void TextManager::clearTexts() { texts.clear(); }

// ------------------------
// Text Properties
// ------------------------
void TextManager::setText(const std::string& id, const std::string& content)
{
    auto it = texts.find(id);
    if (it != texts.end())
        it->second.text.setString(sf::String::fromUtf8(content.begin(), content.end()));
}

void TextManager::setPosition(const std::string& id, const sf::Vector2f& pos)
{
    if (texts.contains(id)) {
        texts[id].text.setPosition(pos);
        if (texts[id].hasShadow)
            texts[id].shadow.setPosition(pos + texts[id].shadow.getPosition());
    }
}

void TextManager::setColor(const std::string& id, const sf::Color& color)
{
    if (texts.contains(id)) texts[id].text.setFillColor(color);
}

void TextManager::setOutline(const std::string& id, float thickness, const sf::Color& color)
{
    if (texts.contains(id)) {
        texts[id].text.setOutlineThickness(thickness);
        texts[id].text.setOutlineColor(color);
    }
}

void TextManager::setCharacterSize(const std::string& id, unsigned int size)
{
    if (texts.contains(id))
        texts[id].text.setCharacterSize(size);
}

void TextManager::setStyle(const std::string& id, sf::Text::Style style)
{
    if (texts.contains(id))
        texts[id].text.setStyle(style);
}

void TextManager::centerText(const std::string& id, float x, float y)
{
    auto it = texts.find(id);
    if (it == texts.end()) return;

    sf::FloatRect bounds = it->second.text.getLocalBounds();
    it->second.text.setOrigin(bounds.left + bounds.width / 2.f,
        bounds.top + bounds.height / 2.f);
    it->second.text.setPosition(x, y);

    if (it->second.hasShadow)
    {
        it->second.shadow.setOrigin(bounds.left + bounds.width / 2.f,
            bounds.top + bounds.height / 2.f);
        it->second.shadow.setPosition(x + it->second.shadow.getPosition().x,
            y + it->second.shadow.getPosition().y);
    }
}

void TextManager::setShadow(const std::string& id, const sf::Color& color, const sf::Vector2f& offset)
{
    if (!texts.contains(id)) return;
    ManagedText& mt = texts[id];
    mt.hasShadow = true;
    mt.shadow = mt.text;
    mt.shadow.setFillColor(color);
    mt.shadow.move(offset);
}

void TextManager::setVisible(const std::string& id, bool visible)
{
	if (texts.contains(id))
	    texts[id].visible = visible;
}

void TextManager::setAllVisible(bool visible)
{
    for (auto& [id, mt] : texts)
    {
        mt.visible = visible;
	}
}

// ------------------------
// Animation
// ------------------------
void TextManager::animateTextFade(const std::string& id, sf::Color start, sf::Color end,
    float duration, bool loop)
{
    if (!texts.contains(id)) return;
    ManagedText& mt = texts[id];
    mt.animation.type = TextAnimation::Type::Fade;
    mt.animation.startColor = start;
    mt.animation.endColor = end;
    mt.animation.duration = duration;
    mt.animation.elapsed = 0.f;
    mt.animation.loop = loop;
    mt.text.setFillColor(start);
}

void TextManager::animateTextScale(const std::string& id, float start, float end,
    float duration, bool loop)
{
    if (!texts.contains(id)) return;
    ManagedText& mt = texts[id];
    mt.animation.type = TextAnimation::Type::Scale;
    mt.animation.startScale = start;
    mt.animation.endScale = end;
    mt.animation.duration = duration;
    mt.animation.elapsed = 0.f;
    mt.animation.loop = loop;
    mt.text.setScale(start, start);
}

void TextManager::updateAnimations(float dt)
{
    for (auto& [id, mt] : texts)
    {
        if (mt.animation.type == TextAnimation::Type::None) continue;

        mt.animation.elapsed += dt;
        float t = std::min(mt.animation.elapsed / mt.animation.duration, 1.f);

        if (mt.animation.type == TextAnimation::Type::Fade)
        {
            sf::Color newColor = mt.animation.startColor + sf::Color(
                static_cast<sf::Uint8>((mt.animation.endColor.r - mt.animation.startColor.r) * t),
                static_cast<sf::Uint8>((mt.animation.endColor.g - mt.animation.startColor.g) * t),
                static_cast<sf::Uint8>((mt.animation.endColor.b - mt.animation.startColor.b) * t),
                static_cast<sf::Uint8>((mt.animation.endColor.a - mt.animation.startColor.a) * t)
            );
            mt.text.setFillColor(newColor);
        }
        else if (mt.animation.type == TextAnimation::Type::Scale)
        {
            float scale = mt.animation.startScale + (mt.animation.endScale - mt.animation.startScale) * t;
            mt.text.setScale(scale, scale);
        }

        if (t >= 1.f)
        {
            if (mt.animation.loop)
                mt.animation.elapsed = 0.f;
            else
                mt.animation.type = TextAnimation::Type::None;
        }
    }
}

// ------------------------
// Drawing
// ------------------------
void TextManager::draw(sf::RenderTarget& target)
{
    for (auto& [id, mt] : texts)
    {
        if (!mt.visible) continue;

        if (mt.hasShadow)
            target.draw(mt.shadow);
        target.draw(mt.text);
    }
}

sf::Text* TextManager::get(const std::string& id)
{
    if (texts.contains(id)) return &texts[id].text;
    return nullptr;
}
