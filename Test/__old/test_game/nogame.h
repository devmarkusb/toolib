// Copyright (C) 2011 Markus Borris

//!
/**
*/
//! \file

#pragma once
#ifndef NOGAME_H_INCL
#define NOGAME_H_INCL

#include <iostream>
#include "Toolib/consts.h"
#include "cgame.h"

namespace nog
{
	class Nogame : public too::game::IMain
	{
	public:
		virtual ~Nogame(){}
		//template<class C> static IMain& getInstance<C>(){}
	protected:
		virtual void intro_init(){std::cout << "### INTRO nogame ###\n";}
		virtual void outro_cleanup(){std::cout << "### OUTRO nogame ###\n";}

		//! \returns false on user exit and true to start game().
		virtual bool menu()
		{
			std::cout << "\n M E N U\n\n";
			std::cout << "1) Play\n";
			std::cout << "2) Exit\n";
			std::cout << "Choose and press ENTER: ";
			char c;
			try{
				std::cin >> c;
			}
			catch(...)
			{
				std::cout << "Bad input! Exiting...";
				return false;
			}
			if (c == '1')
			{
				return true;
			}
			else
				return false;
		}
		//! \returns false on user exit (so gameplay provides a direct exit skipping the menu).
		virtual bool game()
		{
			CGame g;
			if (!g.game())
				return false;
			return true;
		}

		virtual int HandleMainExceptions(std::exception&) throw() { return too::C_I_PROG_EXIT_FAILURE; }
		virtual int mainTerminationByError() throw() { return too::C_I_PROG_EXIT_FAILURE; }
	};
} // nog

#endif