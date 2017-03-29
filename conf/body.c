int main(int argc, char ** argv) {
	if(argc > 1) {
		FILE * fp = fopen(argv[1], "r");
		if(fp == NULL) {
			fprintf(stderr, "File %s: no such file or directory. The stdin will be used\n", argv[1]);
			return EXIT_FAILURE;
		} else {
			yyin = fp;
		}
	}
	yylex();
	return EXIT_SUCCESS;
}
