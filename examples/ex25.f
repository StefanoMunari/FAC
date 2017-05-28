/* example: nested if-else */
fract f = [1|1];



if (true XOR true){
	f = f + [1|2];
	if (false){
		f = f + [1|4];
		print (f);
	}else{
		f = f + [1|3];
		print (f);
	}
	print (f);
}else{
	f = f + [1|1];
	if (false){
		f = f + [1|4];
		print (f);
	}else{
		f = f + [1|3];
		print (f);
	}
	print (f);
}

print(f);
