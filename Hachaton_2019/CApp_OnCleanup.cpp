﻿//==============================================================================
#include "CApp.h"

//==============================================================================
void CApp::OnCleanup() {
	SDL_DestroyTexture(picturePlayer);
	SDL_DestroyRenderer(Renderer_Display);
    SDL_Quit();
}

//==============================================================================
