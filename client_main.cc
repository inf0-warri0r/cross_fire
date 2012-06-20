/*
*Author :Tharindra Galahena
*Project:cross fire - network file sharing application for linux
*Date   :20/06/2012
*/

#include "file_dir_struct.h"
#include "sock.h"
#include "gui.h"
#include "file_handle.h"
#include "ui.cc"

int main(int argc, char* argv[]){
	set_save_dir(getenv("HOME"));
	Gtk::Main kit(argc, argv);
	window_main window;
	set_windows(&window);
	kit.run(window);
	return 0;
}
