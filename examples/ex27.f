/* example: trick the compiler */
bool b0=true;
bool b1=false;
fract f = [5|1];



if(b0 && b1){
	f=f-[1|1];
}else{
	f=f+[1|1];
}

print(f);