/*int main(int argc, char * argv[]) {
  if (argc > 1) {
    FILE * fp = fopen(argv[1], "r");
    if (fp == NULL) {
      err_handler(argv[1], FAC_STANDARD_ERROR);
      return EXIT_FAILURE;
    } 
    else 
      yyin = fp;
  }
  yylex();
  int err_code = fclose(yyin);
  if(err_code == EOF)
    err_handler(argv[1], FAC_STANDARD_ERROR);
  return EXIT_SUCCESS;
}*/
