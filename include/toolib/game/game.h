// 2011

//! \file

#ifndef GAME_H_aubzstex3zubte632yet7
#define GAME_H_aubzstex3zubte632yet7

#include "../config.h"
#include "ul/ul.h"
#include <exception>


namespace mb::too
{
//! Alias toog::.
namespace game
{
//####################################################################################################################
// IMain

//! Implementation interface for the frame program of a game.
/** Gameplay itself shall be started with game() implemented with the help of special
    IGame-implementation, \see IGame. Please study main() and mainloop() for the basic logic.*/
class IMain
{
public:
    virtual ~IMain() = default;
    //! The only function to be called. returns main exit code of the program.
    int main()
    {
        try
        {
            intro_init();
            mainloop();
            outro_cleanup();
            return ul::prog_exit_success;
        }
        catch (std::exception& e)
        {
            return handleMainExceptions(e);
        }
        catch (...)
        {
            return mainTerminationByError();
        }
    }

protected:
    virtual void intro_init() = 0;
    virtual void outro_cleanup() = 0;

    //! \return false on user exit and true to start game().
    virtual bool menu() = 0;
    //! \return false on user exit (so gameplay provides a direct exit skipping the menu).
    /** Usual implementation: \code
        SomeGameImplementingIGame g;
        if (!g.game())
            return false;
        return true;
        \endcode */
    virtual bool game() = 0;

    //! Should return the main exit code.
    virtual int handleMainExceptions(std::exception&) throw() = 0;
    //! Should return the main exit code.
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


//####################################################################################################################
// IGame

//! Implementation interface for actual gameplay, supposed to start its loop by the game() member.
class IGame
{
public:
    virtual ~IGame() = default;

    enum EEndType
    {
        ET_ERROR = -1,
        ET_USER_EXIT = 0,
        ET_GAME_OVER = 1,
        ET_GAME_WON = 2,
    };

    //! The only function to be called. Starts gameplay.
    /** \return false on user exit (so gameplay provides a direct exit skipping e.g. a framework menu).
        or error treated like user exit.*/
    bool game()
    {
        EEndType et;
        while (gameloop(et))
            ;
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
    //! \return false if game ends. This can only happen by user exit.
    virtual bool getEvents() = 0;
    virtual void runAI() = 0;
    virtual void moveEnemies() = 0;
    //! \return false if game ends. In that case et contains either ET_GAME_OVER or ET_GAME_WON.
    virtual bool resolveCollisions(EEndType& et) = 0;
    virtual void drawGraphics() = 0;
    virtual void playSounds() = 0;
    //! \return true if game can be resumed, or otherwise has to be terminated.
    virtual bool handleGameExceptions(std::exception&) = 0;
    //! This function should somehow get access to detailed error information and decide whether:
    /** 1) the game can resume from the menu stage
        2) the program has to exit as though the user would have opted for
        3) it even throws again.
        \return true for 1), false for 2), or throws.*/
    virtual bool gameTerminationByError() = 0;
    virtual void gameTerminationByUserExit() = 0;
    virtual void gameTerminationByGameOver() = 0;
    virtual void gameTerminationByGameWon() = 0;

private:
    //! \return false if game ends. Afterwards et contains reason for ending.
    bool gameloop(EEndType& et)
    {
        try
        {
            if (!getEvents())
            {
                et = ET_USER_EXIT;
                return false;
            }
            runAI();
            moveEnemies();
            if (!resolveCollisions(et))
                return false;
            drawGraphics();
            playSounds();
        }
        catch (std::exception& e)
        {
            if (!handleGameExceptions(e))
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
} // namespace game
} // namespace mb::too

namespace mb
{
namespace toog = too::game;
} // namespace mb

#endif
