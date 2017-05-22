/* RELOP */
fract f0;
fract f1;
bool b0;
bool b1; 

b0 = (f0 < f1 && f1 < f0) || ((f0 + [1|3]) <= (f1 + f0)) -> (f0 + f1) >= f1; 
