void printString(char*);
void readString(char*);
void readSector(char*, int);
void handleInterrupt21(int, int, int , int );

char line[80];
char buffer[512];

int main(){
makeInterrupt21();
putInMemory(0xB000, 0x8000, 'H');
putInMemory(0xB000, 0x8001, 0x4);

putInMemory(0xB000, 0x8002, 'e');
putInMemory(0xB000, 0x8003, 0x4);

putInMemory(0xB000, 0x8004, 'l');
putInMemory(0xB000, 0x8005, 0x4);

putInMemory(0xB000, 0x8006, 'l');
putInMemory(0xB000, 0x8007, 0x4);

putInMemory(0xB000, 0x8008, 'o');
putInMemory(0xB000, 0x8009, 0x4);



putInMemory(0xB000, 0x800A, ' ');
putInMemory(0xB000, 0x800B, 0x4);



putInMemory(0xB000, 0x800C, 'W');
putInMemory(0xB000, 0x800D, 0x4);

putInMemory(0xB000, 0x800E, 'o');
putInMemory(0xB000, 0x800F, 0x4);

putInMemory(0xB000, 0x8010, 'r');
putInMemory(0xB000, 0x8011, 0x4);

putInMemory(0xB000, 0x8012, 'l');
putInMemory(0xB000, 0x8013, 0x4);

putInMemory(0xB000, 0x8014, 'd');
putInMemory(0xB000, 0x8015, 0x4);


interrupt(0x21,0,0,0,0);

//readSector(buffer, 30);
//printString(buffer);
//printString("Enter a line: \0");
 //readString(line);
 //printString(line);
// printString("A7massy Nagad!\0");
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

void handleInterrupt21(int ax, int bx, int cx, int dx){
	printString("Interrupt 21\0");
}
