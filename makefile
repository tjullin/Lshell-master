install:
	gcc lshell.h lshell.c make_prompt.c read_command.c -o lshell run_builtin_command.c
uninstall:
	rm lshell
