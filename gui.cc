class window_download : public Gtk::Window{
	
public:
  window_download();
  virtual ~window_download();
  void update_progress(double fraction);
  void set_file(int id);
  bool on_timeout();
protected:
  void on_button_recv();
  void on_button_dir();
  void on_button_connect();
  void on_button_download();
  void on_button_close();
  
  int f_id;
  
  Gtk::Label label_head; 
  Gtk::Label label_pre1; 
  Gtk::Label label_pre2; 
  Gtk::Label label_head2; 
  
  Gtk::HBox box_head;
  Gtk::HBox box_pre;
  Gtk::HBox box_progress;
  Gtk::HButtonBox box_buttons;
  Gtk::VBox box_main;

  Gtk::ProgressBar progress_download;

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
  
  int f_id;
  
  Gtk::Label label_head; 
  Gtk::Label label_lip; 
  Gtk::Label label_rip; 
  
  Gtk::TextView text_lip;
  Gtk::TextView text_rip;
  
  Gtk::VBox box_main;
  
  Gtk::Table tabel_buttons;
  
  Gtk::Button button_connect;
  Gtk::Button button_close;
  
  //window_main *parent;
  
};
class window_main : public Gtk::Window{
	
public:
  window_main();
  virtual ~window_main();
  void add_data_dirs();
  void add_data_files();
  window_download *get_ref();
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
  
  
  Gtk::HBox box_form;
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
  window_download w;
  window_connect con;
};

