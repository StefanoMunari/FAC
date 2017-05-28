/* RELOP */
fract f0 = [1|3];
fract f1 = [2|3];
bool b0 = (f0 < f1 && f1 < f0) || ((f0 + [1|3]) <= (f1 + f0)) -> (f0 + f1) >= f1; 


