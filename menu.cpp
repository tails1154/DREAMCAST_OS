/* Tsunami for KallistiOS ##version##

   genmenu.cpp
   Copyright (C)2003 Megan Potter

   The included font (typewriter.txf) was pulled from the dcplib example
   in ../../cpp/dcplib.

*/

/*

This example shows off the generic menu class. It only exercises a very
small subset of the possible functionality of genmenu, but it shows the
basics.

*/

#include <kos.h>
#include <math.h>
#include <tsu/genmenu.h>
#include <tsu/font.h>

#include <tsu/drawables/label.h>
#include <tsu/anims/logxymover.h>
#include <tsu/anims/expxymover.h>
#include <tsu/anims/alphafader.h>
#include <tsu/triggers/death.h>

#include <memory>

#include "keytest.h"
extern uint16		*vram_s;
int x, y, o;

class MyMenu : public GenericMenu {
public:
    MyMenu(std::shared_ptr<Font> fnt) {
        // Offset our scene so 0,0,0 is the screen center with Z +10
        m_scene->setTranslate(Vector(320, 240, 10));

        // Set a green background
        setBg(0.2f, 0.4f, 0.2f);

        m_white = Color(1, 1, 1, 1);
        m_gray = Color(1, 0.7f, 0.7f, 0.7f);

        // Setup three labels and have them zoom in.
        m_options[0] = std::make_shared<Label>(fnt, "Keyboard test", 24, true, true);
        m_options[0]->setTranslate(Vector(0, 400, 0));
        m_options[0]->animAdd(std::make_shared<LogXYMover>(0, 0));
        m_options[0]->setTint(m_white);
        m_scene->subAdd(m_options[0]);

        m_options[1] = std::make_shared<Label>(fnt, "Do Thing 2", 24, true, true);
        m_options[1]->setTranslate(Vector(0, 400 + 400, 0));
        m_options[1]->animAdd(std::make_shared<LogXYMover>(0, 24));
        m_options[1]->setTint(m_gray);
        m_scene->subAdd(m_options[1]);

		m_options[2] = std::make_shared<Label>(fnt, "Do Thing 3", 24, true, true);
        m_options[2]->setTranslate(Vector(0, 400 + 400 + 400, 0));
        m_options[2]->animAdd(std::make_shared<LogXYMover>(0, 48));
        m_options[2]->setTint(m_gray);
        m_scene->subAdd(m_options[2]);

        m_options[3] = std::make_shared<Label>(fnt, "Quit", 24, true, true);
        m_options[3]->setTranslate(Vector(0, 400 + 400 + 400, 0));
        m_options[3]->animAdd(std::make_shared<LogXYMover>(0, 72));
        m_options[3]->setTint(m_gray);
        m_scene->subAdd(m_options[3]);

        m_cursel = 0;
    }

    virtual ~MyMenu() {
    }

    virtual void inputEvent(const Event & evt) {
        if(evt.type != Event::EvtKeypress)
            return;

        switch(evt.key) {
            case Event::KeyUp:
                m_cursel--;

                if(m_cursel < 0)
                    m_cursel += 4;

                break;
            case Event::KeyDown:
                m_cursel++;

                if(m_cursel >= 4)
                    m_cursel -= 4;

                break;
            case Event::KeySelect:
                printf("user selected option %d\n", m_cursel);
				if(m_cursel == 0) {
					   for(y = 0; y < 480; y++)
        for(x = 0; x < 640; x++) {
            int c = (x ^ y) & 255;
            vram_s[y * 640 + x] = ((c >> 3) << 12)
                                  | ((c >> 2) << 5)
                                  | ((c >> 3) << 0);
        }

    /* Set our starting offset to one letter height away from the 
       top of the screen and two widths from the left */
    o = (640 * BFONT_HEIGHT) + (BFONT_THIN_WIDTH * 2);
	/* Test with ISO8859-1 encoding */
    bfont_set_encoding(BFONT_CODE_ISO8859_1);
	bfont_draw_str(vram_s + o, 640, 1, "Loading...");
					initkeyboardtest();
				}

                if(m_cursel == 3)
                    startExit();
                break;
            default:
                printf("Unhandled Event Key\n");
                break;
        }

        for(int i = 0; i < 4; i++) {
            if(i == m_cursel)
                m_options[i]->setTint(m_white);
            else
                m_options[i]->setTint(m_gray);
        }
    }

    virtual void startExit() {
        // Apply some expmovers to the options.
	auto m = std::make_shared<ExpXYMover>(0, 1, 0, 400);
        m->triggerAdd(std::make_shared<Death>());
        m_options[0]->animAdd(m);

	m = std::make_shared<ExpXYMover>(0, 1.2, 0, 400);
        m->triggerAdd(std::make_shared<Death>());
        m_options[1]->animAdd(m);

	m = std::make_shared<ExpXYMover>(0, 1.4, 0, 400);
        m->triggerAdd(std::make_shared<Death>());
        m_options[2]->animAdd(m);
	m = std::make_shared<ExpXYMover>(0, 1.6, 0, 400);
        m->triggerAdd(std::make_shared<Death>());
        m_options[3]->animAdd(m);

        GenericMenu::startExit();
    }


    Color       m_white, m_gray;
    std::shared_ptr<Label>   m_options[4];
    int     m_cursel;
};
int main(int argc, char **argv) {

    // Guard against an untoward exit during testing.
    cont_btn_callback(0, CONT_START | CONT_A | CONT_B | CONT_X | CONT_Y,
                      (cont_btn_callback_t)arch_exit);

    // Get 3D going
    pvr_init_defaults();

    // Load a font
    auto fnt = std::make_shared<Font>("/rd/typewriter.txf");

    // Create a menu
    auto mm = std::make_shared<MyMenu>(fnt);

    // Do the menu
    mm->doMenu();


    // Ok, we're all done! The std::shared_ptrs will take care of mem cleanup.

    return 0;
}

