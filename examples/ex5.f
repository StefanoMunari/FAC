/* example: while + nested if*/
bool condition;
bool aux;
aux = true;
condition = true;

while(condition){
	if(aux){
		aux = false
	}else{
		skip;
	}
	if(!aux){
		condition = false;
	}else{
		skip;
	}
}