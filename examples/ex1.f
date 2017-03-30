/*This is a simple F program */

bool condition;
fract a;
fract b;
fract c;

condition = true;
a = [1|2];
b = [ 2 | 3 ];
c = -b + a + b;
if(condition){
	print(a);
} else {
	skip;
}

while(condition){
	print(a);
	condition = false;
}

print(a, a)