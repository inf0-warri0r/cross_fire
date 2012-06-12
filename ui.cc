#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <gtkmm.h>

#include "recc.cc"
#include "gui.cc"

using namespace std;
bool done = true;
void set_tit(double t, bool done);

window_download::window_download() :

	label_head("inf0_warri0r", Gtk::ALIGN_CENTER, Gtk::ALIGN_CENTER),
	label_pre1("lip :   ", Gtk::ALIGN_CENTER, Gtk::ALIGN_RIGHT),
	label_pre2("rip :   ", Gtk::ALIGN_CENTER, Gtk::ALIGN_RIGHT),
	label_head2("inf0_warri0r", Gtk::ALIGN_CENTER, Gtk::ALIGN_CENTER),
	box_head(Gtk::ORIENTATION_VERTICAL, 0),
	box_pre(Gtk::ORIENTATION_HORIZONTAL, 0),
	box_progress(Gtk::ORIENTATION_HORIZONTAL, 0),
	box_buttons(Gtk::BUTTONBOX_END),
	box_main(false, 0),

	button_download("download"),
	button_close("close"),
	ms("wait",Gtk::BUTTONS_CLOSE )
	{
	set_title("inf0_warri0r");
	set_border_width(5);
	set_default_size(500, 200);
	//set_resizable(false);
	add(box_main);
	
	
	button_download.signal_clicked().connect( sigc::mem_fun(*this,
			  &window_download::on_button_download) );
	
	button_close.signal_clicked().connect( sigc::mem_fun(*this,
			  &window_download::on_button_close) );
			  
	box_head.pack_start(label_head);
	box_main.pack_start(box_head);  
	
	box_pre.pack_start(label_pre1);
	box_pre.pack_start(label_pre2);
	box_main.pack_start(box_pre);  
	
	box_progress.pack_start(progress_download);
	
	box_main.pack_start(box_progress);  
	
	//box_buttons.pack_start(label_head2);
	box_buttons.pack_start(button_download);
	box_buttons.pack_start(button_close);
	box_main.pack_start(box_buttons);  
	//box_main.pack_start(box_views);
	update_progress(0.0);
	show_all_children();
}

window_download::~window_download()
{
}
void window_download::on_button_close()
{
	progress_download.set_fraction(0.0);
	set_stop();
    hide();
}
void window_download::update_progress(double fraction){

	double b = progress_download.get_fraction();
	cout << "====   " << b << endl;
	progress_download.set_fraction(fraction);
	//while (g_main_context_iteration(NULL, FALSE));
	progress_download.show();
}
void window_download::set_file(int id){
	f_id = id;
}
void window_download::on_button_download(){
	cout << "aa " << f_id << endl;
	button_download.set_sensitive(true);
	download(f_id, set_tit);
}
void window_download::set_dowload_sen(bool enable){
	button_download.set_sensitive(enable);
}


window_connect::window_connect() :

	label_head("inf0_warri0r", Gtk::ALIGN_CENTER, Gtk::ALIGN_CENTER),
	label_lip("lip :   ", Gtk::ALIGN_CENTER, Gtk::ALIGN_RIGHT),
	label_rip("rip :   ", Gtk::ALIGN_CENTER, Gtk::ALIGN_RIGHT),

	box_main(false, 0),
	tabel_buttons(2,4),
	button_connect("connect"),
	button_close("close")
	{
	set_title("inf0_warri0r");
	set_border_width(5);
	set_default_size(300, 100);
	//set_resizable(false);
	add(box_main);
	scrwin_lip.add(text_lip);
	scrwin_lip.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_NEVER );
	
	scrwin_rip.add(text_rip);
	scrwin_rip.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_NEVER);
	
	tabel_buttons.set_homogeneous(false);
	box_main.pack_start(tabel_buttons, Gtk::PACK_SHRINK);
	tabel_buttons.attach(label_head, 0, 2, 0, 1);
	tabel_buttons.attach(label_lip, 0, 1, 1, 2);
	tabel_buttons.attach(scrwin_lip, 1, 2, 1, 2);

	tabel_buttons.attach(label_rip, 0, 1, 2, 3);
	tabel_buttons.attach(scrwin_rip, 1, 2, 2, 3);
	tabel_buttons.attach(button_connect, 0, 2, 3, 4);
	
	button_connect.signal_clicked().connect( sigc::mem_fun(*this,
			  &window_connect::on_button_connect) );
	
			  
	show_all_children();
}

window_connect::~window_connect()
{
}
void window_connect::on_button_connect(){
	Glib::RefPtr< Gtk::TextBuffer> lip = (text_lip.get_buffer());
	cout << lip -> get_text() << endl;
	Glib::RefPtr< Gtk::TextBuffer> rip = (text_rip.get_buffer());
	cout << rip -> get_text() << endl;
	
	//int ok = init((char *)(rip -> get_text()).c_str(), 
				 // (char *)(lip -> get_text()).c_str());
	int ok = init("127.0.0.1","127.0.0.1");			  
	if(ok == 1){
		hide();
	}
}

window_main::window_main() :

	
	//box_form(Gtk::ORIENTATION_VERTICAL, 0),
	box_views(Gtk::ORIENTATION_HORIZONTAL, 0),
	//box_buttons(Gtk::ORIENTATION_HORIZONTAL, 0),
	box_buttons(Gtk::BUTTONBOX_END),
	box_main(false, 0),
	button_in("goto dir"),
	button_recv("download"),
	button_connect("connect")
	{
	set_title("inf0_warri0r");
	set_border_width(5);
	set_default_size(500, 600);
	//set_resizable(false);
	add(box_main);
	
	button_in.signal_clicked().connect( sigc::mem_fun(*this,
				  &window_main::on_button_dir) );
	
	button_recv.signal_clicked().connect( sigc::mem_fun(*this,
				  &window_main::on_button_recv));
	
	button_connect.signal_clicked().connect( sigc::mem_fun(*this,
				  &window_main::on_button_connect));
				  
	box_buttons.pack_start(button_in);
	box_buttons.pack_start(button_recv);
	box_buttons.pack_start(button_connect);
	box_main.pack_start(box_buttons, Gtk::PACK_SHRINK);  
	
	tm_dirs = Gtk::ListStore::create(columns);
	tv_dirs.set_model(tm_dirs);
	 
	tv_dirs.append_column("ID", columns.id);
	tv_dirs.append_column("Name", columns.name);
	ts_dirs = tv_dirs.get_selection();

	scrwin_dirs.add(tv_dirs);
	scrwin_dirs.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
	box_views.pack_start(scrwin_dirs);
	  
	tm_files = Gtk::ListStore::create(columns);
	tv_files.set_model(tm_files);
	 
	tv_files.append_column("ID", columns.id);
	tv_files.append_column("Name", columns.name);
	tv_files.append_column("size", columns.size);
	ts_files = tv_files.get_selection();
	scrwin_files.add(tv_files);
	scrwin_files.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
	box_views.pack_start(scrwin_files);
	box_main.pack_start(box_views);
	show_all_children();
}

window_main::~window_main()
{
	send_msg(-3);
	sleep(0.5);
	close_sock();
}

void window_main::on_button_recv(){

	Gtk::TreeModel::iterator iter;
	iter = ts_files->get_selected();
	if(iter){
		Gtk::TreeModel::Row row = *iter;
		w.set_title("download");
		w.set_file(row[columns.id] - 1);
		w.update_progress(0.0);
		w.show();
	}
}
void window_main::on_button_dir(){
	Gtk::TreeModel::iterator iter;
	iter = ts_dirs->get_selected();
	if(iter){
		Gtk::TreeModel::Row row = *iter;
		cd((row[columns.id]*(-1)) - 3);
		reset();
		tm_dirs -> clear();
		tm_files -> clear();
		add_data_dirs();
		add_data_files();
	}else{
		reset();
		tm_dirs -> clear();
		tm_files -> clear();
		add_data_dirs();
		add_data_files();
	}
}

void window_main::on_button_connect(){
	con.show_all();
}
void window_main::add_data_dirs(){
	dirs *p = re_st2();
	Gtk::TreeModel::Row row;
	while(p != NULL){
		cout << p -> id << endl;
		row = *(tm_dirs->append());
		row[columns.id] = p -> id + 1;
		row[columns.name] = p -> name;
		p = p -> next;
	}
}
void window_main::add_data_files(){
	files *p = re_st();
	Gtk::TreeModel::Row row;
	while(p != NULL){
		cout << p -> id << endl;
		row = *(tm_files->append());
		row[columns.id] = p -> id + 1;
		row[columns.name] = p -> name;
		stringstream size;
		
		if(p -> size >= 1024*1024){
			size << p -> size/(1024*1024); 
			row[columns.size] = size.str() + " MB";
		}else if(p -> size >= 1024){
			size << p -> size/1024 ;
			row[columns.size] = size.str() + " kB";
		}else{
			size << p -> size;
		    row[columns.size] = size.str() + " bytes";
		}
		p = p -> next;
	}
}
window_download *window_main::get_ref(){
	return &w;
}

window_download *w;
void set_tit(double f, bool done){
	if(!done){
		w -> set_title("downloading...");
		w -> update_progress(f);
		w -> set_dowload_sen(false);
		w -> show();
		//w -> set_sensitive(false);
		
	}else{
		w -> update_progress(1.0);
		//w -> set_sensitive(true);
		w -> set_title("done");
		w -> set_dowload_sen(true);
		//w -> show();
	}
}
int main(int argc, char* argv[]){
  Gtk::Main kit(argc, argv);
	window_main window;
	//w = &window;
	w = window.get_ref();
  kit.run(window);
  return 0;
}
