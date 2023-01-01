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

	qpl::cubic_generator random_range;
	random_range.set_random_range(tick * 0.5, tick * 1.5);
	random_range.set_speed(0.5);

	qpl::f64 random_tick = tick * qpl::random(0.5, 1.5);
	qpl::small_clock clock;
	qpl::small_clock lcontrol_clock;
	bool lcontrol_once_pressed = false;
	bool boost = false;

	bool autoclicker = false;
	qpl::small_clock frame_time;
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
			random_range.set_random_range(tick * 0.5, tick * 1.5);
		}
		if (lcontrol_pressed) {
			lcontrol_clock.reset();
			lcontrol_once_pressed = true;
		}

		if (autoclicker) {
			random_range.update(frame_time.elapsed_f());
			auto delta = random_range.get();

			if (boost) delta /= 4;
			if (clock.has_elapsed_reset(delta)) {
				qpl::winsys::click_left_mouse();
			}
		}
		frame_time.reset();
	}

}
catch (std::exception& any) {
	qpl::println("caught exception:\n", any.what());
	qpl::system_pause();
}