/* example: simple if */
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

if (((b0 || b1) -> b2) && b3){
	print (f);
}else{
	skip;
}

if (((b0 || b1) -> b2) && b3){
	print (b0);
}else{
	skip;
}
