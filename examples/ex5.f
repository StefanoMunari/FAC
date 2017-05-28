/* example: while + nested if*/
bool condition = true;
bool aux = true;


while(condition){
	if(aux){
		aux = false;
	}else{
		aux = true;
	}
	if(!aux){
		condition = false;
	}else{
		condition = true;
	}
}
