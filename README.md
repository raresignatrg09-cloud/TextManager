# TextManager

`TextManager` is a small utility class for managing and rendering multiple
`sf::Text` objects in **SFML**. It centralizes text creation, styling,
visibility control, shadows, and simple time-based animations.

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

## Overview

Texts are stored internally and accessed by **string IDs**.

Each managed text contains:
- an `sf::Text`
- an optional shadow (`sf::Text`)
- a visibility flag
- one active animation (`Fade` or `Scale`)

All texts are rendered using a single `draw()` call.

---

## Font Management

### `bool loadFont(const std::string& path)`

Loads a font from file and stores it internally.

- The **first loaded font** is used for all texts
- Returns `false` if loading fails

### `void clearFonts()`

Removes all loaded fonts.

---

## Text Management

### `void addText(const std::string& id,
                const std::string& content,
                unsigned int size,
                const sf::Vector2f& position)`

Creates a new text entry.

- Does nothing if no font is loaded
- Uses UTF-8 conversion via `sf::String::fromUtf8`

### `void removeText(const std::string& id)`

Removes a text by its ID.

### `void clearTexts()`

Removes all texts.

---

## Text Properties

### `void setText(const std::string& id, const std::string& content)`

Updates the text string.

### `void setPosition(const std::string& id, const sf::Vector2f& position)`

Moves the text.  
If the text has a shadow, the shadow is moved accordingly.

### `void setColor(const std::string& id, const sf::Color& color)`

Sets the fill color.

### `void setOutline(const std::string& id,
                   float thickness,
                   const sf::Color& color)`

Sets outline thickness and color.

### `void setCharacterSize(const std::string& id, unsigned int size)`

Sets character size.

### `void setStyle(const std::string& id, sf::Text::Style style)`

Sets SFML text style.

### `void centerText(const std::string& id, float x, float y)`

Centers the text using its local bounds and moves it to `(x, y)`.  
If a shadow exists, its origin and position are updated as well.

### `void setShadow(const std::string& id,
                  const sf::Color& color,
                  const sf::Vector2f& offset)`

Enables a shadow by duplicating the text and applying an offset.

### `void setVisible(const std::string& id, bool visible)`

Shows or hides a single text.

### `void setAllVisible(bool visible)`

Shows or hides all texts.

### `sf::Text* get(const std::string& id)`

Returns a pointer to the internal `sf::Text`, or `nullptr` if the ID is not found.

---

## Animation System

Each text can have **only one active animation at a time**.

Animations are time-based and must be updated every frame.

### Supported Animation Types

- **Fade** – interpolates between two colors
- **Scale** – interpolates between two scale values

---

### `void animateTextFade(const std::string& id,
                        sf::Color start,
                        sf::Color end,
                        float duration,
                        bool loop = false)`

Starts a fade animation.

- Resets elapsed time
- Immediately sets the start color

---

### `void animateTextScale(const std::string& id,
                         float start,
                         float end,
                         float duration,
                         bool loop = false)`

Starts a scale animation.

- Resets elapsed time
- Immediately sets the start scale

---

### `void updateAnimations(float dt)`

Updates all active animations.

- Must be called every frame
- `dt` is the frame delta time in seconds

---

## Rendering

### `void draw(sf::RenderTarget& target)`

Draws all visible texts.

- Shadows are drawn first
- Invisible texts are skipped

---

## Notes

- All texts use the **same font** (the first loaded one)
- Only one animation per text is supported
- Shadows are implemented as duplicated `sf::Text` objects
- No internal Z-ordering is applied (draw order follows insertion order)

---

## License

Free to use for any purpose.
