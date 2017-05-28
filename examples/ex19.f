/* IFF AND XOR */


fract f0 = [2|3];
fract f1 = [4|3];

bool b0 = (f0 < f1) XOR (f1 < f0);
bool b1 = (f0 + f1 + f1 + f1 < [100|33]) IFF false;
