#!/bin/bash

quartus_map --64bit --read_settings_files=on --write_settings_files=off fprint_1_1 -c fprint_1_1
quartus_fit --64bit --read_settings_files=on --write_settings_files=off fprint_1_1 -c fprint_1_1
quartus_asm --64bit --read_settings_files=on --write_settings_files=off fprint_1_1 -c fprint_1_1
