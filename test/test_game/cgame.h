// Copyright (C) 2011 Markus Borris

//!
/**
*/
//! \file

#pragma once
#ifndef CGAME_H_INCL
#define CGAME_H_INCL

#include <sstream>
#include <iostream>
#include <windows.h>
#include "Toolib/cons_w32/catch_exit.h"
#include "Toolib/log.h"
#include "Toolib/LOGDEF.h"
#include "Toolib/game.h"

namespace nog
{
	class CGame : public too::game::IGame
	{
	public:
		virtual ~CGame(){}

	protected:
		//! \returns false if game ends. This can only happen by user exit.
		virtual bool GetEvents()
		{
			std::cout << "GetEvents()\n";
			Sleep(1000);
			if (too::con::W32ConsExitCatcher.RequestedExit())
				return false;
			return true;
		}
		virtual void RunAI()
		{
			std::cout << "RunAI()\n";
			Sleep(1000);
		}
		virtual void MoveEnemies()
		{
			std::cout << "MoveEnemies()\n";
			Sleep(1000);
		}
		//! \returns false if game ends. In that case \param et contains either ET_GAME_OVER or ET_GAME_WON.
		virtual bool ResolveCollisions(EEndType&)
		{
			std::cout << "ResolveCollisions()\n";
			Sleep(1000);
			return true;
		}
		virtual void DrawGraphics()
		{
			std::cout << "DrawGraphics()\n";
			Sleep(1000);
		}
		virtual void PlaySounds()
		{
			std::cout << "PlaySounds()\n";
			Sleep(1000);
		}
		//! \returns true if game can be resumed, or otherwise has to be terminated.
		virtual bool HandleGameExceptions(std::exception&)
		{
			std::cout << "HandleGameExceptions()\n";
			Sleep(1000);
            return true;
		}
		//! This function should somehow get access to detailed error information and decide whether:
		/** 1) the game can resume from the menu stage
			2) the program has to exit as though the user would have opted for
			3) it even throws again.
			It \returns true for 1), false for 2), or throws.*/
		virtual bool gameTerminationByError()
		{
			std::cout << "gameTerminationByError()\n";
			Sleep(1000);
			return false;
		}
		virtual void gameTerminationByUserExit()
		{
			std::cout << "gameTerminationByUserExit()\n";
			Sleep(1000);
		}
		virtual void gameTerminationByGameOver()
		{
			std::cout << "gameTerminationByGameOver()\n";
			Sleep(1000);
		}
		virtual void gameTerminationByGameWon()
		{
			std::cout << "gameTerminationByGameWon()\n";
			Sleep(1000);
		}
	};
} // nog

#endif