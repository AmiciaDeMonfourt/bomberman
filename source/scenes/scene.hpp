#ifndef __SCENE_HPP__
#define __SCENE_HPP__

#include "..\common.hpp"

class Scene
{
    public:

        virtual void Logic(void) = 0;
        
        virtual void Render(void) = 0;
        
        virtual void Event(SDL_Event* userEvent) = 0;
        
        virtual ~Scene(void);
    
};

#endif