#include "recc.cc"

void add_to_file_queue(char *name, int size);
void add_to_dir_queue(char *name);
void get_files_list();
void get_dirs_list();
void send_msg(int m);
char *check(int id);
void reset();
void cd(int num);
void download(string dir, int num, void (*progress_func)(double t, bool done));
files *refresh_files_queue();
dirs *refresh_dirs_queue();
void close_sock();
void set_stop();
int init(char *r_ip, char *l_ip);
