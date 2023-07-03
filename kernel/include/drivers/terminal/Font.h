#ifndef VIPER_DRIVERS_TERMINAL_FONT_H
#define VIPER_DRIVERS_TERMINAL_FONT_H 1

#include <stdint.h>

namespace haf
{
    class Module;
}

namespace drivers
{
    namespace terminal
    {
        enum class CharacterSet
        {
            Unknown,
            ASCII,
        };

        class Font
        {
        public:
            Font(void* data, uint32_t sizeX, uint32_t sizeY, CharacterSet characterSet);

            void* getData() const;

            uint32_t getSizeX() const;
            uint32_t getSizeY() const;

            CharacterSet getCharacterSet() const;

            static Font FromSFF(void* address, uint64_t size);
            static Font FromSFF(haf::Module module);

        private:
            void* mData{ nullptr };

            uint32_t mSizeX{ 0 };
            uint32_t mSizeY{ 0 };

            CharacterSet mCharacterSet{ CharacterSet::Unknown };
        };
    }
}

#endif