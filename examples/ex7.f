/* example: while + nested print + nested if */
bool condition = true;
fract f = [5|1];



while(condition){
	print(f);
	f = f - [1|1];
	if (f == [1|1]){
		condition = false;
	}
}
