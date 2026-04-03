#include <vector>
#include <iostream>
#include <stdio.h>
class Buttons{
	public:
	typedef enum{
		LEFT,
		RIGHT,
		BOTH,
		NONE
	} BUTTONSTATE;
	typedef enum{
			LEFT_PRESS,
			RIGHT_PRESS,
			LEFT_RELEASE,
			RIGHT_RELEASE
		} BUTTONEVENTS;
		void init();
		void getButtonEvents();
		BUTTONSTATE getButtons();
		std::vector<BUTTONEVENTS> buttonEvents;
	private:
		int button_l;
		int button_r;
		bool button_l_state;
		bool button_r_state;
};