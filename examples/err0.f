bool b = false;
fract f = [1|1];
b = true;
f = [1|3];
/* problematic assignment - wrong error */
b = b;
/* problematic assignment - wrong error */
f = f; 
/* problematic assignment - correct error */
f = b;
/*
Risolvere con:
1) metto start condition nell'analizzatore lessicale e quindi disambiguo i tipi
durante l'analisi lessicale
2) creo un non terminale EXPR generico per tutti i tipi di espressioni e faccio
type checking dopo (durante analisi semantica) => OK questo
*/
