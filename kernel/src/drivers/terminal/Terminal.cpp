#include <drivers/terminal/Terminal.h>

#include <kernel/CoreFramework.h>

namespace drivers
{
    namespace terminal
    {
        Terminal::Terminal(kernel::Framework& coreFramework, Font font)
            : mCoreFramework(coreFramework)
            , mFont(font)
            , mSizeX((coreFramework.getRenderer()->getWidth()  * 4))
            , mSizeY((coreFramework.getRenderer()->getHeight() * 4))
        {
        }

        Font Terminal::getFont() const
        {
            return mFont;
        }

        void Terminal::putCharacter(char character, uint32_t xPosition, uint32_t yPosition, Color foreground, Color background)
        {
            char* characterData = (char*)mFont.getData() + character * mFont.getSizeX();
            mCoreFramework.getRenderer()->drawBitmap(characterData, mFont.getSizeX(), mFont.getSizeY(), xPosition * mFont.getSizeX(), yPosition * mFont.getSizeY(), foreground, background);
        }

        void Terminal::putCharacter(char character)
        {
            switch(character)
            {
                case '\n':
                {
                    mPositionX = 0;
                    mPositionY += 1;
                    break;
                }
                default:
                {
                    putCharacter(character, mPositionX, mPositionY, mForeground, mBackground);
                    if (++mPositionX >= mSizeX)
                    {
                        mPositionX = 0;
                        mPositionY += 1;
                    }
                    break;
                }
            }
        }

        void Terminal::putString(const char* data, size_t size)
        {
            for (size_t i = 0; i < size; i++)
            {
                putCharacter(data[i]);
            }
        }

        void Terminal::print(const char* data)
        {
            for (size_t i = 0; data[i] != 0; i++)
            {
                putCharacter(data[i]);
            }
        }
    }
}