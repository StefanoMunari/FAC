/* example: simple if-else */
fract f;

f = [1|1];

if (true){
	f = f + [1|2];
	print (f);
}else{
	f = f + [1|1];
	print (f);
}

if (false){
	f = f + [1|4];
	print (f);
}else{
	f = f + [1|3];
	print (f);
}

print(f);