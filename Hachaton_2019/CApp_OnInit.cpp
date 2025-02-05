﻿//==============================================================================
#include "CApp.h"

//==============================================================================
bool CApp::OnInit() {
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        return false;
    }	


	if (IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG) == 0)
	{
		return false;
	}

	// Создаём окно
	if((Wind_Display = SDL_CreateWindow("Wow", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, 0) ) == NULL) {
        return false;
    }
	//SDL_SetWindowFullscreen(Wind_Display, SDL_WINDOW_FULLSCREEN);
	Renderer_Display = SDL_CreateRenderer(Wind_Display, -1, 0);
	WindowSize::Init(Wind_Display);

	SDL_Point a{ 1000,1000 };
	SDL_Point b{ 10,10 };
	Bullet.SetTexture(Renderer_Display, 5, &a, &b);

	menu = Menu(Wind_Display, Renderer_Display);
	//иниц  текстур для гаме
	picturePlayer = CSurface::OnLoad(Renderer_Display, "GameActor.jpg");
	AnimPicturePlayer = CSurface::OnLoad(Renderer_Display, "TextAnim.png");;

	texturePlayer = AnimationTexture(Renderer_Display, AnimPicturePlayer, 9, 4, NADO, NADO);
	//texturePlayer.Oscillate = true;
	texturePlayer.InGame.h = 145;
	texturePlayer.InGame.w = 145;
	//texturePlayer = Texture(picturePlayer, 0, 0, 25, 25);
    return true;
}

//==============================================================================
