#include "texture_map.hpp"
#include "..\game.hpp"
#include <vector>

void TextureMap::Init(void)
{
    SDL_Renderer* renderer = Game::GetInstance()->GetRenderer();

    std::vector<std::string> paths = 
    {
        "assets/entity/black-hole.png",
        "assets/entity/earth.png",
        "assets/entity/enemy1.png",
        "assets/entity/enemy2.png",
        "assets/entity/enemy3.png",
        "assets/entity/enemy4.png",
        "assets/entity/enemy5.png",
        "assets/entity/enemy6.png",
        "assets/entity/bomb.png",
        "assets/map/brick1.png",
        "assets/map/floor1.png",
        "assets/map/wall.png",
        "assets/map/explosionCenter.png",
        "assets/map/explosionSide.png",
        "assets/map/explosionEnd.png",
        "assets/map/title.png",
        "assets/map/vignette.png",
        "assets/boosts/boosts.png"
    };

    std::vector<std::string> keys = 
    {
        "black-hole",
        "earth",
        "enemy1",
        "enemy2",
        "enemy3",
        "enemy4",
        "enemy5",
        "enemy6",
        "bomb",
        "brick",
        "floor",
        "wall",
        "explosionCenter",
        "explosionSide",
        "explosionEnd",
        "title",
        "vignette",
        "boosts"
    };

    for(int i = 0; i < paths.size(); i++)
    {
        Texture* texture = new Texture(paths[i], renderer);
        mTextureMap[keys[i]] = texture;
    }    

    SDL_SetTextureAlphaMod(mTextureMap["vignette"]->GetTexture(), 128);

    SDL_SetTextureBlendMode(mTextureMap["explosionCenter"]->GetTexture(), SDL_BLENDMODE_BLEND);
    SDL_SetTextureBlendMode(mTextureMap["explosionSide"]->GetTexture(), SDL_BLENDMODE_BLEND);
    SDL_SetTextureBlendMode(mTextureMap["explosionEnd"]->GetTexture(), SDL_BLENDMODE_BLEND);

}

void TextureMap::CleanUp(void)
{
    std::cout << "TextureMap::CleanUp - begin" << std::endl;
    for(auto it = mTextureMap.begin(); it != mTextureMap.end(); ) 
    {
        it->second->CleanUp();
        delete it->second;
        it = mTextureMap.erase(it);
    }
    std::cout << "TextureMap::CleanUp - end" << std::endl;
}

Texture* TextureMap::Get(std::string key)
{
    if(mTextureMap.find(key) == mTextureMap.end())
        throw std::invalid_argument(std::string("invalid texture key - ") + key);
    
    return mTextureMap[key];
}

std::map<std::string, Texture*> TextureMap::mTextureMap;
