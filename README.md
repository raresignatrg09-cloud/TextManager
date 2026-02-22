# TextManager

`TextManager` is a utility class for managing and rendering multiple
`sf::Text` objects in **SFML**. It centralizes text creation, styling,
alignment, visibility control, shadows, and simple time-based animations.

---

## Files
- TextManager.h
- TextManager.cpp

---

## Dependencies

- **SFML** (Graphics module)
- **C++20**
  - Uses `std::unordered_map::contains`

---

## Description

Texts are stored and accessed using **string identifiers**.

Each managed text consists of:

- a main `sf::Text`
- an optional shadow (`sf::Text`)
- visibility flag
- text alignment (`Left`, `Center`, `Right`)
- a single active animation (`Fade` or `Scale`)

All texts are rendered through a single `draw()` call.

---

## Font Handling

### `bool loadFont(std::string_view id, const std::filesystem::path& path)`

Loads a font from a file and stores it internally with a string ID.  

- Returns `false` if loading fails.
- Allows multiple fonts to be loaded and used independently.

### `void clearFonts()`

Clears all loaded fonts.

---

## Text Handling

### `void createText(std::string_view id,
                    std::string_view fontID,
                    std::string_view content,
                    unsigned int size,
                    sf::Vector2f position)`

Creates a new text entry.  

- Throws if the font ID does not exist.
- Supports Unicode content via `std::string`.

### `void removeText(std::string_view id)`

Removes a text by ID.

### `void clearTexts()`

Removes all texts.

### `bool contains(std::string_view id) const`

Checks if a text with the given ID exists.

---

## Text Properties

### `void setString(std::string_view id, std::string_view content)`

Updates the text string.

### `void setPosition(std::string_view id, sf::Vector2f position)`

Moves the text.  
If a shadow exists, it is repositioned accordingly.

### `void setColor(std::string_view id, sf::Color color)`

Sets the fill color.

### `void setOutline(std::string_view id,
                   float thickness,
                   sf::Color color)`

Sets outline thickness and color.

### `void setCharacterSize(std::string_view id, unsigned int size)`

Sets the character size.

### `void setStyle(std::string_view id, sf::Text::Style style)`

Sets the SFML text style.

### `void setAlignment(std::string_view id, TextManager::Alignment alignment)`

Sets text alignment (`Left`, `Center`, `Right`).  
Updates the origin automatically.

### `void setShadow(std::string_view id,
                  sf::Color color,
                  sf::Vector2f offset)`

Enables a shadow by duplicating the text and applying an offset.

### `void setVisible(std::string_view id, bool visible)`

Shows or hides a specific text.

---

## Animation System

Each text supports **one active animation at a time**.

Animations are time-based and must be updated every frame via `update()`.

### Supported Animations

- **Fade** — interpolates between two colors
- **Scale** — interpolates between two scale values

### `void animateFade(std::string_view id,
                    sf::Color start,
                    sf::Color end,
                    sf::Time duration,
                    bool loop = false)`

Starts a fade animation.  

- Resets animation time
- Immediately applies the start color

### `void animateScale(std::string_view id,
                     float start,
                     float end,
                     sf::Time duration,
                     bool loop = false)`

Starts a scale animation.  

- Resets animation time
- Immediately applies the start scale

### `void update(sf::Time dt)`

Updates all active animations.

- Must be called once per frame
- `dt` is delta time

---

## Rendering

### `void draw(sf::RenderTarget& target) const`

Draws all visible texts.  

- Shadows are drawn first
- Invisible texts are skipped

---

## Notes

- Supports multiple fonts, each identified by a string ID.
- Only one animation can run per text.
- Shadows are implemented as duplicated `sf::Text` objects.
- Text alignment automatically adjusts the origin.
- No explicit layering; draw order follows insertion order.

---

## License

Free to use for any purpose.
