int main(int argc, char * argv[]) {
  if (argc > 1) {
    FILE * fp = fopen(argv[1], "r");
    if (fp == NULL) {
      fprintf(stderr, "File %s: no such file or directory\n", argv[1]);
      return EXIT_FAILURE;
    } else 
      yyin = fp;
    
  }
  yylex();
  int err_code = fclose(yyin);
  if(err_code == EOF) {
	fprintf(stderr, "Error while cloing input file \n");
  }
  return EXIT_SUCCESS;
}
