/* example: nested while*/
bool condition=true;

fract f = [5|1];



while(condition){
	while(f > [0|1]){
		condition = false;
		while(f > [0|1]){
			f=f-[1|1];
		}
		condition=true;
	}
	condition=false;
}

print(f);
print(condition);