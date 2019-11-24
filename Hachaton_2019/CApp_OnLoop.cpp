﻿//==============================================================================
#include "CApp.h"

//==============================================================================
void CApp::OnLoop() {
	if (game) {
		//получаем смещение игрока
		CSurface::CoordObjectIn25D(Wind_Display, &texturePlayer, game->get_player_1(), game);
		texturePlayer.OnAnimation(); //анимация запуск

		printf_s("%d %d %d %d\n", texturePlayer.InGame.x, texturePlayer.InGame.y, texturePlayer.InGame.w, texturePlayer.InGame.h);
	}
}

//==============================================================================
