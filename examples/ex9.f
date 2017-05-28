/* example: simple if-else */
bool b0 = true;
bool b1 = false;
bool b2 = true;
bool b3 = false;
fract f = [1|1];








if (((b0 XOR b1) <-> b2) XOR b3){
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
