#include <qpl/qpl.hpp>

qpl::f64 get_tickspeed() {
	qpl::print("how many clicks / second? > ");
	return 1.0 / qpl::f64_cast(qpl::get_input());
}

int main() try {
	qpl::f64 tick = 1.0 / 10.0;

	qpl::println("press VK_XBUTTON1 to toggle autoclick.");
	qpl::println("press VK_XBUTTON2 to boost.");
	qpl::println("double press LCTR  to change speed.");

	qpl::f64 random_tick = tick * qpl::random(0.5, 1.5);
	qpl::small_clock clock;
	qpl::small_clock lcontrol_clock;
	bool lcontrol_once_pressed = false;
	bool boost = false;

	bool autoclicker = false;
	while (true) {
		bool lcontrol = qpl::winsys::key_holding(VK_LCONTROL);
		bool lcontrol_pressed = qpl::winsys::key_pressed(VK_LCONTROL);


		if (qpl::winsys::key_pressed(VK_XBUTTON1)) {
			autoclicker = !autoclicker;
			if (autoclicker) {
				qpl::println("autoclicker ON");
			}
			else {
				qpl::println("autoclicker OFF");
			}
		}
		if (qpl::winsys::key_pressed(VK_XBUTTON2)) {
			boost = !boost;
		}
		if (lcontrol_once_pressed && lcontrol_pressed && lcontrol_clock.elapsed_f() < 0.5) {
			tick = get_tickspeed();
		}
		if (lcontrol_pressed) {
			lcontrol_clock.reset();
			lcontrol_once_pressed = true;
		}

		if (autoclicker) {
			if (clock.has_elapsed_reset(random_tick)) {
				random_tick = tick * qpl::random(0.5, 1.5);
				if (boost) random_tick /= 4;

				qpl::winsys::click_left_mouse();
			}
		}
	}

}
catch (std::exception& any) {
	qpl::println("caught exception:\n", any.what());
	qpl::system_pause();
}