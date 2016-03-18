void printString(char*);
void readString(char*);
void readSector(char*, int);
void readFile(char*,char*);
void handleInterrupt21(int, int, int , int );

char buffer[13312];

int main(){

	makeInterrupt21();
	interrupt(0x21, 3, "messag\0", buffer, 0); /*read the file into buffer*/
	interrupt(0x21, 0, buffer, 0, 0); /*print out the file*/

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
			interrupt(0x10, 0xE*256 + 0xa, 0, 0, 0);	
			interrupt(0x10, 0xE*256 + 0xd, 0, 0, 0);	

			chars[i] = 0xa;
			i++;
			chars[i] = 0x0;
			i++;
			break;
		}		

		interrupt(0x10, 0xE*256 + c, 0, 0, 0);

		if(c != 0x8){
			chars[i] = c;
			i++;
		}
		else{
			interrupt(0x10, 0xE*256 + ' ', 0, 0, 0);
			interrupt(0x10, 0xE*256 + c, 0, 0, 0);
			i--;
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
	char* origin = *buffer;
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
					readSector(buffer , x);
					buffer = buffer +512;
				}
				else break;
				j++;
			}
			break;
		}
		i = i+32;
	}
	*buffer = *origin;
}

void handleInterrupt21(int ax, int bx, int cx, int dx){
	switch(ax){
		case 0: printString(bx); break;
		case 1: readString(bx); break;
		case 2: readSector(bx,cx); break;
		case 3: readFile(bx,cx);break;
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

