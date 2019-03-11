#ifndef SCAN_H_INCLUDED
#define SCAN_H_INCLUDED

void load_config(const char *scancodes_filename, const char *mnemonic_filename);
int process_scancode(int scancode, char *buffer);


#endif
