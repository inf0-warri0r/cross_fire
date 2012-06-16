#include <gtkmm.h>

class window_download : public Gtk::Window{
	
	public:
		window_download();
		virtual ~window_download();
		void update_progress(double fraction);
		void set_file(int id);
		bool on_timeout();
		void set_dowload_sen(bool enable);
	protected:
		void on_button_recv();
		void on_button_dir();
		void on_button_connect();
		void on_button_download();
		void on_button_close();
		void on_button_choose();
		  
		int f_id;
		   
		Gtk::Label label_head; 
		Gtk::Label label_head2; 
		  
		Gtk::HBox box_head;
		Gtk::HBox box_pre;
		Gtk::HBox box_progress;
		Gtk::HButtonBox box_buttons;
		Gtk::VBox box_main;

		Gtk::ProgressBar progress_download;
			
		Gtk::Button button_choose;
		Gtk::Button button_download;
		Gtk::Button button_close;
		Gtk::MessageDialog ms;
  
};
class window_connect : public Gtk::Window{
	
	public:
		window_connect();
		virtual ~window_connect();
	  
	protected:
		void on_button_connect();
		void on_button_close();
		  
		Gtk::Label label_head; 
		Gtk::Label label_lip; 
		Gtk::Label label_rip; 
		
		
		Gtk::TextView text_lip;
		Gtk::TextView text_rip;
		  
		Gtk::VBox box_main;
		  
		Gtk::Table tabel_buttons;

		Gtk::ScrolledWindow scrwin_lip;
		Gtk::ScrolledWindow scrwin_rip;
		  
		Gtk::Button button_connect;
		Gtk::Button button_close;
	  
};
class window_main : public Gtk::Window{
	
	public:
		window_main();
		virtual ~window_main();
		void add_data_dirs();
		void add_data_files();
		window_download *get_ref_download();
		window_download *get_ref_main();
	protected:
		void on_button_recv();
		void on_button_dir();
		void on_button_connect();
		void on_button_disconnect();
	  
		class ModelColumns : public Gtk::TreeModel::ColumnRecord
		{
			public:

				ModelColumns(){ 
					add(id); 
					add(name); 
					add(size);
				}

				Gtk::TreeModelColumn<unsigned int> id;
				Gtk::TreeModelColumn<Glib::ustring> name;
				Gtk::TreeModelColumn<Glib::ustring> size;
		};

		ModelColumns columns;
		  
		Gtk::Image image_head;
		  
		Gtk::HBox box_headder;
		Gtk::HBox box_views;
		Gtk::HButtonBox box_buttons;
		Gtk::VBox box_main;
		  
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
		Gtk::Button button_disconnect;
		
		window_download w;
		window_connect con;
};

