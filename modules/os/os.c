/*
 * os.c
 * -vk
 *
 * punt extension to some low-level stuff (designed to be a partial replica of
 * cpython's 'os' module
 *
 * <license stuff> (I prefer BSD. I don't care what you do with the code)
*/

#include "../../common.h"

#include <unistd.h>

MFUNC_REPORT {
	char** funcs = (char**) calloc(4, sizeof(char *));

	funcs[0] = "system";
	funcs[1] = "sleep";
	funcs[2] = "access";

/*
	funcs[3] = "yes";
	funcs[4] = "no";
*/

	return funcs;
}

void errq(const char* msg) {
	fprintf(stderr, msg);
	exit(1);
}

MFUNC_PROTO(system) {
	check_argc("system", 1, args);

	if (args->type != P_STR) {
		errq("argument must be a string\n");
	}

	return make_atom(P_NUM, "",
		atom_dupnum(system((char*) args->value)));
}

MFUNC_PROTO(sleep) {
	check_argc("sleep", 1, args);

	if (args->type != P_NUM) {
		errq("argument must be a number\n");
	}

	return make_atom(P_NUM, "",
		atom_dupnum(sleep(*((p_num*) args->value))));
}

MFUNC_PROTO(access) {
	check_argc("access", 2, args);

	if (args->type != P_STR || ((p_atom*) args->next)->type != P_STR) {
		errq("both arguments must be strings\n");
	}

	int amode = 0;
	char* flags = (char*) ((p_atom*) args->next)->value;

	unsigned short it;
	for (it=0; it < strlen(flags); ++it) {
		switch (flags[it]) {
			case 'r':
				amode |= R_OK;
				break;
			case 'w':
				amode |= W_OK;
				break;
			case 'x':
				amode |= X_OK;
				break;
			case 'f':
				amode |= F_OK;
				break;
			default:
				errq("unknown access flag. f,w,r and x are allowed.\n");
		}
	}

	return make_atom(P_NUM, "",
		atom_dupnum(access((char*) args->value, amode)));
}

