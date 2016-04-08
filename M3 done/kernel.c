void printString(char*);
void readString(char*);
void readSector(char*, int);
void readFile(char*,char*);
void handleInterrupt21(int, int, int , int );
void executeProgram(char*, int);
void terminate();
void deleteFile(char* name);
void writeFile(char* name, char* buffer, int secNum);

int i=0;
char buffer[13312];
char buffer1[13312];
char buffer2[13312];
char term[6];

int main(){
	// makeInterrupt21();
	// interrupt(0x21, 3, "messag\0", buffer, 0); /*read the file into buffer*/
	// interrupt(0x21, 0, buffer, 0, 0); /*print out the file*/
	// interrupt(0x21, 4, "tstprg\0", 0x2000, 0);
	// interrupt(0x21, 4, "tstpr2\0", 0x2000, 0);

	// makeInterrupt21();
	// interrupt(0x21, 7, "messag\0", 0, 0); //delete messag
	// interrupt(0x21, 3, "messag\0", buffer, 0); // try to read messag
	// interrupt(0x21, 0, buffer, 0, 0); //print out the contents of buffer

	// buffer2[0]='h';
	// buffer2[1]='e';
	// buffer2[2]='l';
	// buffer2[3]='l';
	// buffer2[4]='o';
	// for(i=5; i<13312; i++){
	// 	buffer2[i]=0x0;
	// }
	// makeInterrupt21();
	// interrupt(0x21,8, "testW\0", buffer2, 1); //write file testW
	// interrupt(0x21,3, "testW\0", buffer1, 0); //read file testW
	// interrupt(0x21,0, buffer1, 0, 0);

	makeInterrupt21();
	interrupt(0x21,0, "Welcome to Team 33
		Os\n\r", 0, 0);
	term[0]='s';
	term[1]='h';
	term[2]='e';
	term[3]='l';
	term[4]='l';
	term[5]='\0';
	interrupt(0x21,4, term, 0x2000, 0);



	while(1){
	}
}

void printString(char* chars){
	int i = 0;
	while(chars[i] != '\0'){
		int AX = 0xE*256 + chars[i];
		interrupt(0x10, AX, 0, 0, 0);
		i++;
	}
}

void readString(char* chars){
	// if(chars.length() < 80)return;
	char c;
	int i = 0;
		while(c = interrupt(0x16,0,0,0,0)){

				if(c == 0xd){
					interrupt(0x10, 0xE*256 + 0xa, 0, 0, 0);//new line
					interrupt(0x10, 0xE*256 + 0xd, 0, 0, 0);//move cursor to the left
					chars[i] = 0xa;
					i++;
					chars[i] = 0xd;	//move cursor to the left
					i++;
					chars[i] = 0x0;
					i++;
					return;
				}
				if(c != 0x8){
					interrupt(0x10, 0xE*256 + c, 0, 0, 0);
					chars[i] = c;
					i++;
				}
				else{
					if(i>0){
						interrupt(0x10, 0xE*256 + c, 0, 0, 0);	//move the cursor back
						interrupt(0x10, 0xE*256 + ' ', 0, 0, 0);	//print empty char
						interrupt(0x10, 0xE*256 + c, 0, 0, 0);	//move the cursor back
						i--;
					}
				}
		}
}

void readSector(char* buffer, int sector){
	int track = div(sector, 36);
	int relSec = mod(sector, 18) + 1;
	int head = mod(div(sector, 18), 2);

	int AH = 2;
	int AL = 1;
	int AX = AH*256+AL;

	char* BX = buffer;

	int CH = track;
	int CL =  relSec;
	int CX = CH*256+CL;

	int DH = head;
	int DL = 0;
	int DX = DH*256+DL;

	interrupt(0x13, AX, BX, CX, DX);
}

void readFile(char* array, char* buffer){
	char temp [512];

	int i = 0;
	// char* origin = *buffer;
	interrupt(0x21, 2, temp, 2, 0);
	while(i<512){
		char c [7];
		int j =0;
		while (j<6){
			c[j] = temp[i+j];
			j++;
		}
		c[6] = '\0';
		if(equals(array , c)==1){
			while(j<32){
				int x = temp[i+j];
				if(x != 0){
					interrupt(0x21, 2, buffer , x);
					buffer = buffer +512;
				}
				else break;
				j++;
			}
			break;
		}
		i = i+32;
	}
	// *buffer = *origin;
}

void executeProgram(char* name, int segment){
	char buff[13312];
	int i = 0;
	readFile(name,buff);
	while(i< 13312){
		putInMemory(segment, i, buff[i]);
		i++;
	}
	launchProgram(segment);
}

void terminate(){
	term[0]='s';
	term[1]='h';
	term[2]='e';
	term[3]='l';
	term[4]='l';
	term[5]='\0';
	interrupt(0x21, 4, term, 0x2000, 0);
}

void writeSector(char* buffer, int sector){
	int track = div(sector, 36);
	int relSec = mod(sector, 18) + 1;
	int head = mod(div(sector, 18), 2);

	int AH = 3;
	int AL = 1;
	int AX = AH*256+AL;

	char* BX = buffer;

	int CH = track;
	int CL =  relSec;
	int CX = CH*256+CL;

	int DH = head;
	int DL = 0;
	int DX = DH*256+DL;

	interrupt(0x13, AX, BX, CX, DX);
}

void writeFile(char* name, char* buffer, int secNum){
	char map [512];
	char dir [512];
	int sectors [26];
	int i = 0;
	int s = 0;
	interrupt(0x21, 2, map, 1, 0);
	interrupt(0x21, 2, dir, 2, 0);

	while(i<512){
		if(map[i] == 0x00){
			sectors[s] = i;
			s++;
			if(s==secNum){
				break;
			}
		}
		i++;
	}
	if(s<secNum){
		interrupt(0x21, 99, dir, 2, 0);//error
	}

	else{
		for (i = 0; i < s; ++i)
		{
			int c = sectors[i];
			map[c] = 0xFF;
		}
		interrupt(0x21, 6, map, 1, 0);//write map
		i = 0;
		for (; i < 512; i+=32)
		{
			if(dir[i] == 0x00){
				int j = i;
				int z = 0;//counter for 6 bytes of name
				while(z<6){
					if (name[z] != '\0'){
						dir[j] = name[j-i];
					}
					else{
						dir[j] = 0x00;
					}
					j++;
					z++;
				}
				for (j=6; j < 32; ++j)
				{
					if(j < secNum+6){
						dir[j+i] = sectors[j-6];
					}
					else{
						dir[j+i] = 0x00;
					}
				}
				break;
			}
		}

		for (i = 0; i < secNum; ++i)
		{
			interrupt(0x21, 6, buffer, sectors[i], 0);
			buffer += 512;
		}
		interrupt(0x21, 6, dir, 2, 0);//write directory

	}
}

void deleteFile(char* name){
	char map [512];
	char dir [512];
	int i = 0;
	int del = 0;
	interrupt(0x21, 2, map, 1, 0);
	interrupt(0x21, 2, dir, 2, 0);

	while (i<512){
		char tempName [7];
		int j = 0;
		while (j<6){
			tempName[j] = dir[i+j];
			j++;
		}
		tempName[6] = '\0';
		if(equals(name, tempName)==1){
			del = 1;
			dir[i] = 0x00;
			while(j<32){
				int x = dir[i+j];
				if(x != 0){
					x++;
					map[x] = 0x00;
				}
				j++;
			}
			break;
		}
		i = i+32;
	}

	interrupt(0x21, 6, map, 1, 0);
	interrupt(0x21, 6, dir, 2, 0);
}

void handleInterrupt21(int ax, int bx, int cx, int dx){
	switch(ax){
		case 0: printString(bx); break;
		case 1: readString(bx); break;
		case 2: readSector(bx,cx); break;
		case 3: readFile(bx,cx);break;
		case 4: executeProgram(bx,cx);break;
		case 5: terminate();break;
		case 6: writeSector(bx,cx);break;
		case 7: deleteFile(bx);break;
		case 8: writeFile(bx,cx,dx);break;
		default: printString("error"); break;
	}
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

int equals(char* x , char* y){
	int i = 0;
	while(x[i] != '\0' && y[i] != '\0'){
		if (x[i] != y[i])
			return 0;
		i++;
	}
	if (x[i] == '\0' && y[i] == '\0')
		return 1;
	else return 0;
}
