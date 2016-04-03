char line[74];
char input1[14];
char input2[7];
char buffer[13312];
char dir[512];
int j;
int i;
int k;
int counter;

int main()
{
	// while(1){
		interrupt(0x21, 0, "SHELL>", 0, 0);
		interrupt(0x21, 1, line, 0, 0);
		j=0;
		i = 1;
		for(i = 1; i<80; i++){
			if(line[i-1] == ' '){
				while(line[i+j] != 0xa){
					input1[j] = line[i+j];
					j++;
				}
				break;
			}
		}
		input1[j]= '\0';
		j=0;
		for(i = 1; i<14; i++){
			if(input1[i-1] == ' '){
				input1[i-1]= '\0';	
				while(input1[i+j] != '\0'){
					input2[j] = input1[i+j];
					j++;
				}
				break;
			}
		}
		input2[j]= '\0';


		if(line[0] == 'v' && line[1]=='i' && line[2]=='e' && line[3]=='w' && line[5] != ' '){ //should return file not found in buffer with k=1
					if( (input1[1] == '\0') || (input1[2] == '\0') 
						|| (input1[3] == '\0') || (input1[4] == '\0') || (input1[5] == '\0') || (input1[6] == '\0') )	{	
						interrupt(0x21, 3, input1, buffer, 0);
					if(buffer[0]=='\0')
						interrupt(0x21, 0, "File is not found!\n\r", 0, 0);
					else
						interrupt(0x21, 0, buffer, 0, 0);
					}
					else
						interrupt(0x21, 0, "Maximum size for a file is 6 characters\n\r", 0, 0);
		}


		else if(line[0]=='e' && line[1]=='x' && line[2]=='e' && line[3]=='c' 
					&& line[4]=='u' && line[5]=='t' && line[6]=='e' && line[8] != ' '){ //program doesn't terminate &&
																			// if file is not found should print and not just try to run it
					if( (input1[1] == '\0') || (input1[2] == '\0') 
						|| (input1[3] == '\0') || (input1[4] == '\0') || (input1[5] == '\0') || (input1[6] == '\0') )
						interrupt(0x21, 4, input1, 0x2000, 0);
					else
						interrupt(0x21, 0, "Maximum size for a file is 6 characters\n\r", 0, 0);
		}


		else if(line[0]=='d' && line[1]=='e' && line[2]=='l' 
					&& line[3]=='e'&& line[4]=='t' && line[5]=='e' && line[7] != ' '){
			if( (input1[1] == '\0') || (input1[2] == '\0') 
						|| (input1[3] == '\0') || (input1[4] == '\0') || (input1[5] == '\0') || (input1[6] == '\0') ){
					interrupt(0x21, 3, input1, buffer, 0);
					interrupt(0x21, 0, buffer, 0, 0);
					if(buffer[0] == '\0')
						interrupt(0x21, 0, "File is not found!\n\r", 0, 0);
					else{
						interrupt(0x21, 7, input1, 0, 0);
						interrupt(0x21, 0, "Deleted succsefully\n\r", 0, 0);
					}
				}
					else
						interrupt(0x21, 0, "Maximum size for a file is 6 characters\n\r", 0, 0);
		}


		else if(line[0]=='d' && line[1]=='i' && line[2]=='r' && line[3]!=' '){
					interrupt(0x21, 0, "FileName : Size(in sectors)\n\r", 0, 0);
					i = 0;
					interrupt(0x21, 2, dir, 2, 0);
					while(i<512){
						j=0;
						counter=0;
						if(dir[i] != 0x00){
							while(j<6 && dir[i+j] != 0x00)
								input1[j++] = dir[i+j];
							while(j<6)
								input1[j++] = ' ';
							k=j+i;
							input1[j++] = ' ';
							input1[j++] = ' ';
							input1[j++] = ' ';
							input1[j++] = ':';
							input1[j++] = ' ';
							while(dir[k++] != 00)
								counter++;
							input2[0]='\0';
							k=1;
							while(counter != 0){
								input2[k++] = '0'+mod(counter,10); 
								counter = div(counter,10);
							}
							k--;
							while(input2[k] != '\0')
								input1[j++] = input2[k--];
							input1[j++] = '\n';
							input1[j++] = '\r';
							input1[j] = '\0';
							interrupt(0x21, 0, input1, 0, 0);
							input1[0]='\0';
						}
						i += 32;
					}
		}
		

		else if(line[0]=='c' && line[1]=='o' && line[2]=='p' && line[3]=='y' && line[5] != ' '){
				if( ((input1[1] == '\0') || (input1[2] == '\0') 
						|| (input1[3] == '\0') || (input1[4] == '\0') || (input1[5] == '\0') || (input1[6] == '\0'))
						&& ((input2[1] == '\0') || (input2[2] == '\0') 
						|| (input2[3] == '\0') || (input2[4] == '\0') || (input2[5] == '\0') || (input2[6] == '\0')) ){
					interrupt(0x21, 3, input1, buffer, 0);
						if(buffer[0]=='\0')
							interrupt(0x21, 0, "File is not found!\n\r", 0, 0);
						else{
							interrupt(0x21,8, input2, buffer, 1);
							interrupt(0x21, 0, "Copied succsefully\n\r", 0, 0);
						}
					}
					else
						interrupt(0x21, 0, "Maximum size for a file is 6 characters or wrong parameters passed\n\r", 0, 0);
		}


		else if (line[0]=='c' && line[1]=='r' && line[2]=='e' && line[3]=='a'
					&& line[4]=='t' && line[5]=='e' && line[7] != ' '){
				if( (input1[1] == '\0') || (input1[2] == '\0') 
						|| (input1[3] == '\0') || (input1[4] == '\0') || (input1[5] == '\0') || (input1[6] == '\0') ){
					interrupt(0x21, 1, buffer, 0, 0);
					counter=0;
					while(buffer[counter++] != '\0');
					j = div(counter,512);
					if(mod(counter,512) != 0)
						j++;
					interrupt(0x21,8, input1, buffer, j);
					interrupt(0x21, 0, "Created succsefully\n\r", 0, 0);
				}
				else
					interrupt(0x21, 0, "Maximum size for a file is 6 characters\n\r", 0, 0);
		}
		
		else interrupt(0x21, 0, "Bad Command!\n\r", 0, 0);

		interrupt(0x21,5, 0, 0, 0);
		// interrupt(0x21,5,0,0,0);
		// buffer[0] = '\0';
		// k=0;
		// input1[0]='\0';
		// input2[0]='\0';
	// }

}

int mod(int a, int b){
	int res = div(a,b);
	res = res*b;
	res = a - res;
	return res;
}

int div(int a, int b){
	int c = 0;
	int res = a-b;
	while (res >= 0){
		c++;
		res -= b;
	}
	return c;
}