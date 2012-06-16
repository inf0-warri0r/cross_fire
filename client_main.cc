#include "sock.h"
#include "gui.h"
#include "rec.h"
#include "ui.cc"

int main(int argc, char* argv[]){
	set_save_dir(getenv("HOME"));
	Gtk::Main kit(argc, argv);
	window_main window;
	set_windows(&window);
	kit.run(window);
	return 0;
}
