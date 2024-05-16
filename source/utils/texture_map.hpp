#ifndef __TEXTURE_MAP_HPP__
#define __TEXTURE_MAP_HPP__

#include "..\common.hpp"
#include "texture.hpp"
#include <map>

class TextureMap
{
    private:

        static std::map<std::string, Texture*> mTextureMap;

    public:

        static void Init(void);

        static void CleanUp(void);

        static Texture* Get(std::string textureKey);
};



#endif