/* example: simple if */
bool b0 = true;
bool b1 = false;
bool b2 = true;
bool b3 = false;
fract f = [1|1];








if (((b0 || b1) -> b2) && b3){
	print (f);
}

if (((b0 || b1) -> b2) && b3){
	print (b0);
}else{
	print(b1);
}
