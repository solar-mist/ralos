#ifndef VIPER_DRIVERS_TERMINAL_TERMINAL_H
#define VIPER_DRIVERS_TERMINAL_TERMINAL_H 1

#include <drivers/terminal/Color.h>
#include <drivers/terminal/Font.h>

#include <stdint.h>
#include <stddef.h>

namespace haf
{
    class Framework;
}

namespace drivers
{
    namespace terminal
    {
        class Terminal
        {
        public:
            Terminal(haf::Framework& hardwareFramework, Font font);

            Font getFont() const;

            uint32_t getPositionX() const;
            uint32_t getPositionY() const;

            uint32_t getSizeX() const;
            uint32_t getSizeY() const;

            void putCharacter(char character, uint32_t xPosition, uint32_t yPosition, Color foreground, Color background);
            void putCharacter(char character);

            void putString(const char* data, size_t size);

            void print(const char* data);

        private:
            haf::Framework& mHardwareFramework;
            
            Font mFont;

            uint32_t mPositionX{ 0 };
            uint32_t mPositionY{ 0 };

            const uint32_t mSizeX{ 0 };
            const uint32_t mSizeY{ 0 };

            Color mForeground{ Colors::White };
            Color mBackground{ Colors::Black };
        };
    }
}

#endif