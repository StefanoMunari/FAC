/* example: nested while*/
bool condition;
fract f;

condition=true;
f = [5|1];

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

f=[7|1];
