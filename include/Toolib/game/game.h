// Markus Borris, 2011
// This file is part of Toolib library. Open source.

//!
/**
*/
//! \file

#ifndef GAME_H_INCL_aubzstex3zubte632yet7
#define GAME_H_INCL_aubzstex3zubte632yet7

#include <exception>
#include "Toolib/consts.h"

namespace too
{
	//! Alias toog::.
	namespace game
	{

		//##############################################################################################################
		// IMain

		//! Implementation interface for the frame program of a game.
		/** Gameplay itself shall be started with game() implemented with the help of special
			IGame-implementation, \see IGame. Please study main() and mainloop() for the basic logic.*/
		class IMain
		{
		public:
			virtual ~IMain(){}
			//! The only function to be called. \returns main exit code of the program.
			int main()
			{
				try
				{
					intro_init();
					mainloop();
					outro_cleanup();
					return prog_exit_success;
				}
				catch (std::exception& e)
				{
					return HandleMainExceptions(e);
				}
				catch (...)
				{
					return mainTerminationByError();
				}
			}
		protected:
			virtual void intro_init() = 0;
			virtual void outro_cleanup() = 0;

			//! \returns false on user exit and true to start game().
			virtual bool menu() = 0;
			//! \returns false on user exit (so gameplay provides a direct exit skipping the menu).
			/** Usual implementation: \code
				SomeGameImplementingIGame g;
				if (!g.game())
					return false;
				return true;
				\endcode */
			virtual bool game() = 0;

			//! Should \return the main exit code.
			virtual int HandleMainExceptions(std::exception&) throw() = 0;
			//! Should \return the main exit code.
			virtual int mainTerminationByError() throw() = 0;
		private:
			void mainloop()
			{
				while (menu())
				{
					if (!game())
						break;
				}
			}
		};


		//##############################################################################################################
		// IGame

		//! Implementation interface for actual gameplay, supposed to start its loop by the game() member.
		class IGame
		{
		public:
			virtual ~IGame(){}

			enum EEndType {
				ET_ERROR		= -1,
				ET_USER_EXIT	= 0,
				ET_GAME_OVER	= 1,
				ET_GAME_WON		= 2,
			};

			//! The only function to be called. Starts gameplay.
			/** \returns false on user exit (so gameplay provides a direct exit skipping e.g. a framework menu).
			    or error treated like user exit.*/
			bool game()
			{
				EEndType et;
				while(gameloop(et));
				switch (et)
				{
				case ET_USER_EXIT:
					gameTerminationByUserExit();
					return false;
				case ET_GAME_OVER:
					gameTerminationByGameOver();
					break;
				case ET_GAME_WON:
					gameTerminationByGameWon();
					break;
				case ET_ERROR:
				default:
					if (!gameTerminationByError())
						return false;
				}
				return true;
			}
		protected:
			//! \returns false if game ends. This can only happen by user exit.
			virtual bool GetEvents() = 0;
			virtual void RunAI() = 0;
			virtual void MoveEnemies() = 0;
			//! \returns false if game ends. In that case \param et contains either ET_GAME_OVER or ET_GAME_WON.
			virtual bool ResolveCollisions(EEndType& et) = 0;
			virtual void DrawGraphics() = 0;
			virtual void PlaySounds() = 0;
			//! \returns true if game can be resumed, or otherwise has to be terminated.
			virtual bool HandleGameExceptions(std::exception&) = 0;
			//! This function should somehow get access to detailed error information and decide whether:
			/** 1) the game can resume from the menu stage
				2) the program has to exit as though the user would have opted for
				3) it even throws again.
				It \returns true for 1), false for 2), or throws.*/
			virtual bool gameTerminationByError() = 0;
			virtual void gameTerminationByUserExit() = 0;
			virtual void gameTerminationByGameOver() = 0;
			virtual void gameTerminationByGameWon() = 0;
		private:
			//! \returns false if game ends. Afterwards \param et contains reason for ending.
			bool gameloop(EEndType& et)
			{
				try
				{
					if (!GetEvents())
					{
						et = ET_USER_EXIT;
						return false;
					}
					RunAI();
					MoveEnemies();
					if (!ResolveCollisions(et))
						return false;
					DrawGraphics();
					PlaySounds();
				}
				catch (std::exception& e)
				{
					if (!HandleGameExceptions(e))
					{
						et = ET_ERROR;
						return false;
					}
				}
				catch (...)
				{
					et = ET_ERROR;
					return false;
				}
				return true;
			}
		};
	} // game
} // too

namespace toog = too::game;

#endif
