#include <reg51.h>
#define uchar unsigned char
#define RR 0x24 //red red
#define RG 0x0c //red green
#define GR 0x21 //green red
#define GG 0x09 //green green
#define LOCK(num) A##num = 1; A##num = 0 //LOCK(8) mean A8 = 1; A8 = 1;
#define START 127
#define GAP 30

sbit A8 = P1^0;
sbit A9 = P1^1;
sbit A10 = P1^2;
sbit A11 = P1^3;
sbit A12 = P1^4;
sbit A13 = P1^5;
sbit A14 = P1^6;
sbit A15 = P1^7;

uchar mtable[]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x27,0x7f,0x6f};

char sec = START, cnt = 0;

void delay(unsigned int ms);
void status1();
void status2();
void status3();
void status4();
void inittimer();

void main(){
	inittimer();	//open timer
	while(1){
		status1();
		status2();
		status3();
		status4();
	}
}

void inittimer() {
	TMOD = 0X01;
 	TL0 = 0xb0;
	TH0 = 0x3c;
	ET0 = 1; EA = 1;
	TR0 = 1;
}

void status1(){
	while( START-GAP < sec && sec <= START ){
		while( START-GAP+2 <= sec && sec <= START-GAP/2 ){
			P0 = mtable[sec-(START-GAP+1)];
			P3 = ~(0x31); LOCK(14);
			P3 = ~(0x07); LOCK(15);
			delay(100);
			P3 = 0xff; LOCK(14); LOCK(15);
		}
		P2 = RR; LOCK(8);
		P2 = RG; LOCK(9);
		P2 = RR; LOCK(10);
		P2 = ( sec <= START-GAP+2 ) ? 0x22 : GR; LOCK(11);
		P2 = RR; LOCK(12);
		P2 = ( sec <= START-GAP+2 ) ? 0x12 : GG; LOCK(13);
	}
}

void status2(){
	while( START-2*GAP < sec && sec <= START-GAP ){
		while( START-2*GAP+2 <= sec && sec <= (START-GAP)-GAP/2 ){
			P0 = mtable[sec-(START-2*GAP+1)];
			P3 = ~(0x08); LOCK(14);
			P3 = ~(0x3e); LOCK(15);
			delay(100);
			P3 = 0xff; LOCK(14); LOCK(15);
		}
		P2 = GR; LOCK(8);
		P2 = ( sec <= START-2*GAP+2 ) ? 0x14 : RG; LOCK(9);
		P2 = RR; LOCK(10);
		P2 = ( sec <= START-2*GAP+2 ) ? 0x14 : RG; LOCK(11);
		P2 = ( sec <= START-2*GAP+2 ) ? 0x22 : GR;	LOCK(12);
		P2 = RR; LOCK(13);
	}
}


void status3(){
	while( START-3*GAP < sec && sec <= START-2*GAP ){
		while( START-3*GAP+2 <= sec && sec <= (START-2*GAP)-GAP/2 ){
			P0 = mtable[sec-(START-3*GAP+1)];
			P3 = ~(0x07); LOCK(14);
			P3 = ~(0x31); LOCK(15);
			delay(100);
			P3 = 0xff; LOCK(14); LOCK(15);
		}
		P2 = ( sec <= START-3*GAP+2 ) ? 0x22 : GR; LOCK(8);
		P2 = RR; LOCK(9);
		P2 = ( sec <= START-3*GAP+2 ) ? 0x12 : GG; LOCK(10);
		P2 = RR; LOCK(11);
		P2 = RG; LOCK(12);
		P2 = RR; LOCK(13);
	}
}

void status4(){
	while( START-4*GAP < sec && sec <= START-3*GAP ){
		while( START-4*GAP+2 <= sec && sec <= (START-3*GAP)-GAP/2 ){
			P0 = mtable[sec-(START-4*GAP+1)];
			P3 = ~(0x3e); LOCK(14);
			P3 = ~(0x08); LOCK(15);
			delay(100);
			P3 = 0xff; LOCK(14); LOCK(15);
		}
		P2 = ( sec <= START-4*GAP+2 ) ? 0x14 : RG; LOCK(8);
		P2 = ( sec <= START-4*GAP+2 ) ? 0x22 : GR; LOCK(9);
		P2 = RR; LOCK(10);
		P2 = GR; LOCK(11);
		P2 = ( sec <= START-4*GAP+2 ) ? 0x14 : RG; LOCK(12);
		P2 = RR; LOCK(13);
	}
}

void delay(unsigned int ms) {
	unsigned int i,j;
	for(i=0;i<ms;i++);
		for(j=0;j<150;j++);
}

void time_ler() interrupt 1 {
	TL0=0xb0; TH0=0x3c;
	cnt++;
	if( cnt == 20 ){
		cnt = 0;
		sec--;
		if( sec == START-4*GAP ){	//count down reset sec, back to status1
			sec = START;
		}
	}
}