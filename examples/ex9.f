/* example: simple if-else */
bool b0;
bool b1;
bool b2;
bool b3;
fract f;

b0 = true;
b1 = false;
b2 = true;
b3 = false;

f = [1|1];

if (((b0 X b1) <-> b2) <> b3){
	print (f);
}else{
	f = f + [1|1];
	print (f);
}

if ((!(b0 || b1) -> b2) && !b3){
	print (b0);
}else{
	f = f + [1|2];
	print (f);
}
