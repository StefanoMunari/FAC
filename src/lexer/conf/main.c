int main(int argc, char * argv[]) {
  if (argc > 1) {
    FILE * fp = fopen(argv[1], "r");
    if (fp == NULL) {
      fprintf(stderr, "File %s: no such file or directory. The stdin will be used\n", argv[1]);
    } else {
      yyin = fp;
      yylex();
      int err_code = fclose(fp);
      if (err_code == EOF) {
        fprintf(stderr, "Error occured while closing the file %s\n", argv[1]);
      }
    }
  }
  yylex();
  return EXIT_SUCCESS;
}
