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

## Requirements

- **SFML** (Graphics module)
- **C++20**
  - Uses `std::unordered_map::contains`

---

## Description

Texts are stored and accessed using **string identifiers**.

Each managed text consists of:
- a main `sf::Text`
- an optional shadow (`sf::Text`)
- a visibility flag
- a single active animation (`Fade` or `Scale`)

All texts are rendered through a single `draw()` call.

---

## Font Handling

### `bool loadFont(const std::string& path)`

Loads a font from file and stores it internally.

- The **first loaded font** is used for all texts
- Returns `false` if loading fails

### `void clearFonts()`

Clears all loaded fonts.

---

## Text Handling

### `void addText(const std::string& id,
                const std::string& content,
                unsigned int size,
                const sf::Vector2f& position)`

Creates a new text entry.

- Does nothing if no font is loaded
- Text is converted from UTF-8 using `sf::String::fromUtf8`

### `void removeText(const std::string& id)`

Removes a text by ID.

### `void clearTexts()`

Removes all texts.

---

## Text Properties

### `void setText(const std::string& id, const std::string& content)`
Updates the text string.

### `void setPosition(const std::string& id, const sf::Vector2f& position)`
Moves the text.  
If a shadow exists, it is repositioned accordingly.

### `void setColor(const std::string& id, const sf::Color& color)`
Sets the fill color.

### `void setOutline(const std::string& id,
                   float thickness,
                   const sf::Color& color)`
Sets outline thickness and color.

### `void setCharacterSize(const std::string& id, unsigned int size)`
Sets the character size.

### `void setStyle(const std::string& id, sf::Text::Style style)`
Sets the SFML text style.

### `void centerText(const std::string& id, float x, float y)`
Centers the text using its local bounds and positions it at `(x, y)`.  
If a shadow exists, its origin and position are updated as well.

### `void setShadow(const std::string& id,
                  const sf::Color& color,
                  const sf::Vector2f& offset)`
Enables a shadow by duplicating the text and applying an offset.

### `void setVisible(const std::string& id, bool visible)`
Shows or hides a specific text.

### `void setAllVisible(bool visible)`
Shows or hides all texts.

### `sf::Text* get(const std::string& id)`
Returns a pointer to the internal `sf::Text`, or `nullptr` if the ID does not exist.

---

## Animation System

Each text supports **one active animation at a time**.

Animations are time-based and must be updated every frame.

### Supported Animations

- **Fade** — interpolates between two colors
- **Scale** — interpolates between two scale values

---

### `void animateTextFade(const std::string& id,
                        sf::Color start,
                        sf::Color end,
                        float duration,
                        bool loop = false)`

Starts a fade animation.

- Resets animation time
- Immediately applies the start color

---

### `void animateTextScale(const std::string& id,
                         float start,
                         float end,
                         float duration,
                         bool loop = false)`

Starts a scale animation.

- Resets animation time
- Immediately applies the start scale

---

### `void updateAnimations(float dt)`

Updates all active animations.

- Must be called once per frame
- `dt` is delta time in seconds

---

## Rendering

### `void draw(sf::RenderTarget& target)`

Draws all visible texts.

- Shadows are drawn first
- Invisible texts are skipped

---

## Notes

- All texts use the same font (first loaded)
- Only one animation can run per text
- Shadows are implemented as duplicated `sf::Text` objects
- No explicit draw ordering or layering is applied

---

## License

Free to use for any purpose.
