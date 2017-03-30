int main(int argc, char * argv[]) {
  if (argc > 1) {
    FILE * fp = fopen(argv[1], "r");
    if (fp == NULL) {
      yytext=argv[1];
      err_handler("File %s: no such file or directory\n", 'S');
      return EXIT_FAILURE;
    } 
    else 
      yyin = fp;
  }
  yylex();
  int err_code = fclose(yyin);
  if(err_code == EOF)
    err_handler("Error while closing input file \n", '.');
  return EXIT_SUCCESS;
}
