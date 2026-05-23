//! \file

#ifndef GAME_H_AUBZSTEX3ZUBTE632YET7
#define GAME_H_AUBZSTEX3ZUBTE632YET7

#include "../config.h"
#include "mb/ul/ul.hpp"
#include <exception>

namespace mb::too {
//! Alias toog::.
namespace game {
//####################################################################################################################
// IMain

//! Implementation interface for the frame program of a game.
/** Gameplay itself shall be started with game() implemented with the help of special
    IGame-implementation, \see IGame. Please study main() and mainloop() for the basic logic.*/
class IMain {
public:
    virtual ~IMain() = default;

    //! The only function to be called. returns main exit code of the program.
    int main() {
        try {
            intro_init();
            mainloop();
            outro_cleanup();
            return ul::prog_exit_success;
        } catch (std::exception& e) {
            return handle_main_exceptions(e);
        } catch (...) {
            return main_termination_by_error();
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
    virtual int handle_main_exceptions(std::exception&) noexcept = 0;
    //! Should return the main exit code.
    virtual int main_termination_by_error() noexcept = 0;

private:
    void mainloop() {
        while (menu()) {
            if (!game())
                break;
        }
    }
};

//####################################################################################################################
// IGame

//! Implementation interface for actual gameplay, supposed to start its loop by the game() member.
class IGame {
public:
    virtual ~IGame() = default;

    enum EEndType {
        et_error = -1,
        et_user_exit = 0,
        et_game_over = 1,
        et_game_won = 2,
    };

    //! The only function to be called. Starts gameplay.
    /** \return false on user exit (so gameplay provides a direct exit skipping e.g. a framework menu).
        or error treated like user exit.*/
    bool game() {
        EEndType et;
        while (gameloop(et))
            ;
        switch (et) {
            case et_user_exit:
                game_termination_by_user_exit();
                return false;
            case et_game_over:
                game_termination_by_game_over();
                break;
            case et_game_won:
                game_termination_by_game_won();
                break;
            case et_error:
            default:
                if (!game_termination_by_error())
                    return false;
        }
        return true;
    }

protected:
    //! \return false if game ends. This can only happen by user exit.
    virtual bool get_events() = 0;
    virtual void run_ai() = 0;
    virtual void move_enemies() = 0;
    //! \return false if game ends. In that case et contains either ET_GAME_OVER or ET_GAME_WON.
    virtual bool resolve_collisions(EEndType& et) = 0;
    virtual void draw_graphics() = 0;
    virtual void play_sounds() = 0;
    //! \return true if game can be resumed, or otherwise has to be terminated.
    virtual bool handle_game_exceptions(std::exception&) = 0;
    //! This function should somehow get access to detailed error information and decide whether:
    /** 1) the game can resume from the menu stage
        2) the program has to exit as though the user would have opted for
        3) it even throws again.
        \return true for 1), false for 2), or throws.*/
    virtual bool game_termination_by_error() = 0;
    virtual void game_termination_by_user_exit() = 0;
    virtual void game_termination_by_game_over() = 0;
    virtual void game_termination_by_game_won() = 0;

private:
    //! \return false if game ends. Afterwards et contains reason for ending.
    bool gameloop(EEndType& et) {
        try {
            if (!get_events()) {
                et = et_user_exit;
                return false;
            }
            run_ai();
            move_enemies();
            if (!resolve_collisions(et))
                return false;
            draw_graphics();
            play_sounds();
        } catch (std::exception& e) {
            if (!handle_game_exceptions(e)) {
                et = et_error;
                return false;
            }
        } catch (...) {
            et = et_error;
            return false;
        }
        return true;
    }
};
} // namespace game
} // namespace mb::too

namespace mb {
namespace toog = too::game;
} // namespace mb

TOO_HEADER_END

#endif
