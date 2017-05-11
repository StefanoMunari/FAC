/* example: geometric series */
fract f0;
fract f1;
fract f2;
fract f3;
fract f4;
fract f5;
fract f6;
fract f7;
fract f8;
fract f9;

fract s;

f0 = [1|2];
f1 = [1|4];
f2 = [1|8];
f3 = [1|16];
f4 = [1|32];
f5 = [1|64];
f6 = [1|128];
f7 = [1|256];
f8 = [1|512];
f9 = [1|1024];

s = f0 + f1 + f2 + f3 + f4 + f5 + f6 + f7 + f8 + f9;

print(s);
