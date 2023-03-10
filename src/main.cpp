#include <qpl/qpl.hpp>

void add_to_startup() {
	qpl::winsys::set_program_launch_on_startup("C:/dev/projects/VisualStudio2022/AutoClicker/QPL/AutoClicker.exe");
}

int main() try {
	qpl::f64 tick = 1.0 / 10.0;

	qpl::println("press VK_XBUTTON1 to toggle autoclick.");
	qpl::println("press VK_XBUTTON2 to boost.");
	qpl::println("double press LCTR  to change speed.");

	qpl::cubic_generator random_range;
	random_range.set_random_range(-1.0, 1.0);
	random_range.set_speed(1.5);

	qpl::animation activate_boost;
	activate_boost.set_duration(2.0);

	qpl::f64 random_tick = tick * qpl::random(0.5, 1.5);
	qpl::small_clock clock;
	qpl::small_clock lcontrol_clock;

	bool lcontrol_once_pressed = false;
	bool boost = false;
	bool autoclicker = false;
	bool disabled = false;
	bool xbutton2_clicked_once = false;
	constexpr bool hide_console = true;

	if (hide_console) {
		qpl::winsys::hide_console();
	}

	qpl::small_clock frame_time;
	qpl::small_clock disable_cooldown;

	while (true) {
		if (autoclicker) {
			qpl::wait(1.0 / 360);
		}
		else {
			qpl::wait(1.0 / 30);
		}

		auto frame_t = frame_time.elapsed_f();
		frame_time.reset();
		activate_boost.update(frame_t);

		bool lcontrol = qpl::winsys::key_holding(VK_LCONTROL);
		bool lcontrol_pressed = qpl::winsys::key_pressed(VK_LCONTROL);

		auto xbutton1 = qpl::winsys::key_pressed(VK_XBUTTON1);
		auto xbutton2 = qpl::winsys::key_pressed(VK_XBUTTON2);

		if (xbutton2) {
			if (xbutton2_clicked_once && disable_cooldown.elapsed_f() < 0.5) {
				disabled = !disabled;
			}
			xbutton2_clicked_once = true;
			disable_cooldown.reset();
		}

		if (!disabled) {
			auto toggle = xbutton1;

			if (lcontrol && xbutton1) {
				return 0;
			}

			if (toggle) {
				autoclicker = !autoclicker;
			}

			if (autoclicker && xbutton2) {
				boost = !boost;
				if (boost) {
					activate_boost.go_forwards();
				}
				else {
					activate_boost.go_backwards();
				}
			}
			if (lcontrol_pressed) {
				lcontrol_clock.reset();
				lcontrol_once_pressed = true;
			}

			if (autoclicker) {
				random_range.update(frame_t);
				auto exp = std::pow(2, random_range.get());
				auto delta = tick * exp;

				delta *= qpl::linear_interpolation(1.0, 0.1, activate_boost.get_curve_progress());
				if (clock.has_elapsed_reset(delta)) {
					qpl::winsys::click_left_mouse();
				}
			}
		}
	}

}
catch (std::exception& any) {
	qpl::println("caught exception:\n", any.what());
	qpl::system_pause();
}