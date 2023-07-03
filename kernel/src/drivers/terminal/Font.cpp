#include <drivers/terminal/Font.h>

#include <haf/fs/Module.h>

namespace drivers
{
    namespace terminal
    {
        Font::Font(void* data, uint32_t sizeX, uint32_t sizeY, CharacterSet characterSet)
            : mData(data)
            , mSizeX(sizeX)
            , mSizeY(sizeY)
            , mCharacterSet(characterSet)
        {
        }

        void* Font::getData() const
        {
            return mData;
        }

        uint32_t Font::getSizeX() const
        {
            return mSizeX;
        }

        uint32_t Font::getSizeY() const
        {
            return mSizeY;
        }

        CharacterSet Font::getCharacterSet() const
        {
            return mCharacterSet;
        }

        Font Font::FromSFF(void* address, uint64_t size)
        {
            if (!address || !size)
            {
                return Font(nullptr, 0, 0, CharacterSet::Unknown);
            }
            else
            {
                char* fontData = static_cast<char*>(address);

                if (fontData[0] != 's')
                {
                    return Font(nullptr, 0, 0, CharacterSet::Unknown);
                }
                if (fontData[1] != 'f')
                {
                    return Font(nullptr, 0, 0, CharacterSet::Unknown);
                }
                if (fontData[2] != 'f')
                {
                    return Font(nullptr, 0, 0, CharacterSet::Unknown);
                }

                const CharacterSet characterSet = static_cast<CharacterSet>(fontData[3]);
                const uint32_t sizeX            = static_cast<uint32_t>(fontData[4]);
                const uint32_t sizeY            = static_cast<uint32_t>(fontData[5]);
                void* const data                = static_cast<void*>(fontData + 6);

                return Font(data, sizeX, sizeY, characterSet);
            }
        }

        Font Font::FromSFF(haf::Module module)
        {
            return FromSFF(module.getAddress(), module.getSize());
        }
    }
}