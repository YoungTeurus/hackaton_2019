﻿//==============================================================================
#include "CApp.h"

//==============================================================================
void CApp::OnRender() {

	menu.Render(Renderer_Display);
	texturePlayer.DrawTexture(Renderer_Display);
	SDL_RenderPresent(Renderer_Display);

	/*
	if (true)//если игра началась
	{

	}
	if (true)//мама мы в менюшке
	{
	}
	*/
}

//==============================================================================
