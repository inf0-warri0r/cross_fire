#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <gtkmm.h>
#include <gtkmm.h>
#include "recc.cc"

using namespace std;

class ExampleWindow : public Gtk::Window
{
public:
  ExampleWindow();
  virtual ~ExampleWindow();
  void add_data_dirs();
  void add_data_files();
protected:
  void on_button_recv();
  void on_button_dir();
  void on_button_connect();

  class ModelColumns : public Gtk::TreeModel::ColumnRecord
  {
  public:

    ModelColumns()
    { add(id); add(name); add(size);}

    Gtk::TreeModelColumn<unsigned int> id;
    Gtk::TreeModelColumn<Glib::ustring> name;
    Gtk::TreeModelColumn<Glib::ustring> size;
  };

  ModelColumns columns;
  
  Gtk::Label label_head; 
  Gtk::Label label_dum1; 
  Gtk::Label label_dum2; 
  Gtk::Label label_lip; 
  Gtk::Label label_rip; 
  
  Gtk::TextView text_lip;
  Gtk::TextView text_rip;
  
  Gtk::HBox box_form;
  Gtk::HBox box_views;
  Gtk::HBox box_buttons;
  Gtk::VBox box_main;

  Gtk::Table tabel_buttons;
  Gtk::ScrolledWindow scrwin_dirs;
  Gtk::ScrolledWindow scrwin_files;
  
  Gtk::TreeView tv_dirs;
  Gtk::TreeView tv_files;
  Glib::RefPtr<Gtk::ListStore> tm_dirs;
  Glib::RefPtr<Gtk::ListStore> tm_files;
  Glib::RefPtr<Gtk::TreeSelection> ts_dirs;
  Glib::RefPtr<Gtk::TreeSelection> ts_files;
  
  Gtk::Button button_in;
  Gtk::Button button_recv;
  Gtk::Button button_connect;
  
  //Glib::RefPtr< Gtk::TextBuffer> lip;
};


ExampleWindow::ExampleWindow() :

	label_head("inf0_warri0r", Gtk::ALIGN_CENTER, Gtk::ALIGN_CENTER),
	label_lip("lip :   ", Gtk::ALIGN_CENTER, Gtk::ALIGN_RIGHT),
	label_rip("rip :   ", Gtk::ALIGN_CENTER, Gtk::ALIGN_RIGHT),
	box_form(Gtk::ORIENTATION_VERTICAL, 0),
	box_views(Gtk::ORIENTATION_HORIZONTAL, 0),
	box_buttons(Gtk::ORIENTATION_HORIZONTAL, 0),
	box_main(false, 0),
	tabel_buttons(5,6),
	button_in("goto dir"),
	button_recv("download"),
	button_connect("connect")
	{
	set_title("inf0_warri0r");
	set_border_width(5);
	set_default_size(500, 600);
	add(box_main);

	tabel_buttons.set_homogeneous(true);
	box_main.pack_start(tabel_buttons, Gtk::PACK_SHRINK);
	tabel_buttons.attach(label_head, 0, 6, 0, 1);
	tabel_buttons.attach(label_lip, 0, 2, 1, 2);
	tabel_buttons.attach(label_rip, 2, 4, 1, 2);
	tabel_buttons.attach(text_lip, 0, 2, 2, 3);
	tabel_buttons.attach(text_rip, 2, 4, 2, 3);
	tabel_buttons.attach(button_connect, 4, 6, 2, 3);
	tabel_buttons.attach(label_dum1, 0, 6, 3, 4);
	tabel_buttons.attach(button_in, 1, 2, 4, 5);
	tabel_buttons.attach(label_dum2, 3, 4, 4, 5);
	tabel_buttons.attach(button_recv, 4, 5, 4, 5);
	
	button_in.signal_clicked().connect( sigc::mem_fun(*this,
				  &ExampleWindow::on_button_dir) );
	
	button_recv.signal_clicked().connect( sigc::mem_fun(*this,
				  &ExampleWindow::on_button_recv));
	
	button_connect.signal_clicked().connect( sigc::mem_fun(*this,
				  &ExampleWindow::on_button_connect));
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

ExampleWindow::~ExampleWindow()
{
}

void ExampleWindow::on_button_recv(){

	Gtk::TreeModel::iterator iter;
	iter = ts_files->get_selected();
	if(iter){
		Gtk::TreeModel::Row row = *iter;
		download(row[columns.id] - 1);
	}
}
void ExampleWindow::on_button_dir(){
	Gtk::TreeModel::iterator iter;
	iter = ts_dirs->get_selected();
	if(iter){
		Gtk::TreeModel::Row row = *iter;
		cd((row[columns.id]*(-1)) - 3);
		tm_dirs -> clear();
		tm_files -> clear();
		add_data_dirs();
		add_data_files();
	}
}
void ExampleWindow::on_button_connect(){
	Glib::RefPtr< Gtk::TextBuffer> lip = (text_lip.get_buffer());
	cout << lip -> get_text() << endl;
	Glib::RefPtr< Gtk::TextBuffer> rip = (text_rip.get_buffer());
	cout << rip -> get_text() << endl;
	
	init((char *)(rip -> get_text()).c_str(), (char *)(lip -> get_text()).c_str());
	tm_dirs -> clear();
	tm_files -> clear();
	add_data_dirs();
	add_data_files();
	
}
void ExampleWindow::add_data_dirs(){
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
void ExampleWindow::add_data_files(){
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
int main(int argc, char* argv[]){
  Gtk::Main kit(argc, argv);
	ExampleWindow window;

  kit.run(window);
  return 0;
}
