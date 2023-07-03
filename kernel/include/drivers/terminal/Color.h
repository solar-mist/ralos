#ifndef VIPER_DRIVERS_TERMINAL_COLOR_H
#define VIPER_DRIVERS_TERMINAL_COLOR_H

#include <stdint.h>

namespace drivers
{
    namespace terminal
    {
        class Color
        {
        public:
            Color()
                :mColorCode(0x0)
            {
            }
            explicit constexpr Color(uint32_t colorCode)
                :mColorCode(colorCode)
            {
            }

            operator uint32_t()
            {
                return mColorCode;
            }
        private:
            uint32_t mColorCode;
        };
        
        namespace Colors
        {
            constexpr Color White = Color(0xFFFFFF);
            constexpr Color Black = Color(0x000000);
            constexpr Color Red   = Color(0xFF0000);
            constexpr Color Green = Color(0x00FF00);
            constexpr Color Blue  = Color(0x0000FF);
        }
    }
}

#endif