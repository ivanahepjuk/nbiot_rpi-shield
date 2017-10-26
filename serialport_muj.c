/*

For compiling:
gcc -o serialport_muj serialport_muj.c && sudo ./serialport_muj

*/



#include <string.h>   /* ERROR Number Definitions           */    	
#include <stdio.h>
#include <fcntl.h>   /* File Control Definitions           */
#include <termios.h> /* POSIX Terminal Control Definitions */
#include <unistd.h>  /* UNIX Standard Definitions 	   */ 
#include <errno.h>   /* ERROR Number Definitions           */
#include <stdbool.h> //boolean
#include <time.h>   //clock get time

//colours
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"


//serialport
//#define SERIALPORT dev/ttyUSB0
#define READ_BUFFER 10
#define CONTINUOUS_READING_LENGTH 9
#define PORT "/dev/ttyACM3"
///////////////////////

void intro(void)
{
	printf(ANSI_COLOR_YELLOW"\n\n\n\t\t\t**********************************************************************");
	printf("\n\t\t\t**                                                                  **");
	printf("\n\t\t\t**  ");    printf(ANSI_COLOR_GREEN"uart SARA tool   Labka Ostrava (i_a)                            " ANSI_COLOR_RESET); printf(ANSI_COLOR_YELLOW"**");
	printf("\n\t\t\t**                                                                  **");
	printf("\n\t\t\t**********************************************************************\n\n" ANSI_COLOR_RESET);
}

///////////////////////

///////////////////////
void menu() 
{
	printf(ANSI_COLOR_YELLOW "\n\n\t\t\t  CHOOSE UART MACRO:" ANSI_COLOR_RESET);
	printf("\n\t\t\t# Press 1 for continuous reading serialport.");
	printf("\n\t\t\t# Press 2 for SARA inicialization.");
	printf("\n\t\t\t# Press 3 cgatt");
	printf("\n\t\t\t# Press 4 eval");
	printf("\n\t\t\t# Press 5 for sending a dummy transmission packet to the network.");
	printf("\n\t\t\t### Press x for EXIT program ###\n>");
}
///////////////////////

time_t time(time_t *t); 

void main(void)
{
	int fd;/*File Descriptor*/
	int c;//char handler
	int index = 0;
	char input_str[7]; //pro cteni z konzole
	//char path [20] = "dev/ttyUSB0";

	bool quit = false;//obsluha hlavniho switche

	intro();       //vypise uvitaci screen



	/**************************************/
	/*		INICIALIZACE SERIOVEHO PORTU		*/
	/**************************************/

	fd = open("/dev/ttyUSB0", O_RDWR | O_NOCTTY);//pokus o otevreni serioveho portu

	if(fd == -1)
		printf(ANSI_COLOR_RED "\n\t\tError in opening serial port :(" ANSI_COLOR_RESET);
	else
		printf(ANSI_COLOR_BLUE "\n\t\tSerial port opened successfully :)" ANSI_COLOR_RESET);

	//Nastaveni serioveho portu:
	struct termios SerialPortSettings;	/* nastaveni termios structury pro otevreny seriovy port (jeho konfigurace)*/
	tcgetattr(fd, &SerialPortSettings);	/* nastavi aktualni konfiguraci serioveho portu */

	/* Baud rate */
	cfsetispeed(&SerialPortSettings,B9600); /* cteni 9600 baud */
	cfsetospeed(&SerialPortSettings,B9600); /* psani 9600 baud */

	/* 8N1 Mod */
	SerialPortSettings.c_cflag &= ~PARENB;   /* zakaze paritu, povoli jeden stop bit   */
	SerialPortSettings.c_cflag &= ~CSTOPB;   /* vyzisti stopB takze nastavi na jeden stop bit */
	SerialPortSettings.c_cflag &= ~CSIZE;	 /* vycisti masku pro pocet bitu*/
	SerialPortSettings.c_cflag |=  CS8;      /* nastavi pocet bitu na 8*/

	SerialPortSettings.c_cflag &= ~CRTSCTS;       /* bez hardwarove kontroly toku */
	SerialPortSettings.c_cflag |= CREAD | CLOCAL; /* povoli cteni, ignoruje modem control lines */

	SerialPortSettings.c_iflag &= ~(IXON | IXOFF | IXANY);          /* zakaze xon/xoff mode flowcontrol pro i/p a o/p */
	SerialPortSettings.c_iflag &= ~(ICANON | ECHO | ECHOE | ISIG);  /* non cannonical mode*/
  SerialPortSettings.c_lflag = 0;                // no signaling chars, no echo,

	SerialPortSettings.c_oflag &= ~OPOST;/*No Output Processing*/

		/* Time outs */
	SerialPortSettings.c_cc[VMIN] = 200; /* precti minimalne 10 znaku */
	SerialPortSettings.c_cc[VTIME] = 10; /* cekej nekonecno  */

	if((tcsetattr(fd,TCSANOW,&SerialPortSettings)) != 0) /* ted ty vsechny atributy nastavi te strukture*/
		printf(ANSI_COLOR_RED ", error in setting attributes.\n"ANSI_COLOR_RESET);  //error check
	else
		printf(ANSI_COLOR_BLUE ", BaudRate = 9600, StopBits = 1, Parity = none\n"ANSI_COLOR_RESET ); //vsecko ok

	/**************************************/
	/*							PROGRAM								*/
	/**************************************/

	

	while(!quit)
	{
	menu();		//vypise menu		
	c = getchar ();		//prepinaci znak pri nastavovani modu
		switch (c)
		{
			case '1': 
			{
				time_t start_time = 0;
				time_t elapsed_time = 0;
				char read_buffer[READ_BUFFER];   /* sem bude ukladat prijate data              */
				int  bytes_read = 0;    /* pocet bytu ktere precte pri volani read() system callu */

				tcflush(fd, TCIFLUSH);   /* zahodi stare data z RX bufferu  */
				printf("Macro n.1 'continuous reading' just started.\n");
				while((c = getchar()) != '\n' && c != EOF)
				;//zahozeni entru
				start_time = time(NULL);
			
				while(1)
				{
					elapsed_time = time(NULL);
			
					if((elapsed_time - start_time) > CONTINUOUS_READING_LENGTH)
					{
						printf("\n--> Macro n.1 'continuous reading' just ended.");	
						menu();	//vypise menu	
						break;
					}
		
					bytes_read = read(fd,&read_buffer,READ_BUFFER); 
					for(int i=0;i<bytes_read;i++)	 // vypise buffer
			    	printf("%c",read_buffer[i]);
				}
			break;
			}

		case '2':
		{
			char read_buffer[50];   /* sem bude ukladat prijate data              */
			int  bytes_readed = 0;    /* pocet bytu ktere precte pri volani read() system callu */

			tcflush(fd, TCIFLUSH);   /* zahodi stare data z RX bufferu  */
			printf("Macro n.2 'SARA INITIALIZATION' just started.\n");
			while((c = getchar()) != '\n' && c != EOF)
			;//zahozeni entru
				
			//poslu retezec
			write (fd, "AT+CGMI\r\n", 9);
      								printf("Sent command:");
											printf(ANSI_COLOR_YELLOW "\nAT+CGMI\n" ANSI_COLOR_RESET);  
											printf("\nReceived response:");          
											//prectu znak dokud nedojde K a enter
											//char byte_readed;
											bytes_readed = read(fd,&read_buffer,50);
											for(int j=0;j<bytes_readed; j++)
												printf(ANSI_COLOR_BLUE "%c"ANSI_COLOR_RESET,  read_buffer[j] );		
												printf("\n\n");	
			
											tcflush(fd, TCIFLUSH);   /* zahodi stare data z RX bufferu  */
				//dalsi cmd
				//poslu retezec
			write (fd, "AT+CGMM\r\n", 9);
printf("Sent command:");
			printf(ANSI_COLOR_YELLOW "\nAT+CGMM\n"ANSI_COLOR_RESET);  
											printf("\nReceived response:");          
											//prectu znak dokud nedojde K a enter
											//char byte_readed;
											bytes_readed = read(fd,&read_buffer,50);
											for(int j=0;j<bytes_readed; j++)
												printf(ANSI_COLOR_BLUE "%c"ANSI_COLOR_RESET,  read_buffer[j] );		
											printf("\n\n");	
			
											tcflush(fd, TCIFLUSH);   /* zahodi stare data z RX bufferu  */
			//dalsi cmd
			//poslu retezec
			write (fd, "AT+CGMR\r\n", 9);
printf("Sent command:");
			printf(ANSI_COLOR_YELLOW "\nAT+CGMR\n"ANSI_COLOR_RESET);  
											printf("\nReceived response:");          
											//prectu znak dokud nedojde K a enter
											//char byte_readed;
											bytes_readed = read(fd,&read_buffer,50);
											for(int j=0;j<bytes_readed; j++)
												printf(ANSI_COLOR_BLUE "%c"ANSI_COLOR_RESET,  read_buffer[j] );		
											printf("\n\n");	
			
											tcflush(fd, TCIFLUSH);   /* zahodi stare data z RX bufferu  */
			//dalsi cmd

			//poslu retezec
			write (fd, "AT+CGSN=1\r\n", 11);
											printf("Sent command:");
											printf(ANSI_COLOR_YELLOW "\nAT+CGSN=1\n"ANSI_COLOR_RESET);  
											printf("\nReceived response:");          
											//prectu znak dokud nedojde K a enter
											//char byte_readed;
											bytes_readed = read(fd,&read_buffer,50);
											for(int j=0;j<bytes_readed; j++)
												printf(ANSI_COLOR_BLUE "%c"ANSI_COLOR_RESET,  read_buffer[j] );			
											printf("\n\n");	
			
											tcflush(fd, TCIFLUSH);   // zahodi stare data z RX bufferu  
			//dalsi cmd
			//poslu retezec

			write (fd, "AT+CGSN=2\r\n", 11);
											printf("Sent command:");
											printf(ANSI_COLOR_YELLOW "\nAT+CGSN=2\n"ANSI_COLOR_RESET);  
											printf("\nReceived response:");          
											//prectu znak dokud nedojde K a enter
											//char byte_readed;
											bytes_readed = read(fd,&read_buffer,50);
											for(int j=0;j<bytes_readed; j++)
												printf(ANSI_COLOR_BLUE "%c"ANSI_COLOR_RESET,  read_buffer[j] );			
											printf("\n\n");	
			
											tcflush(fd, TCIFLUSH);   // zahodi stare data z RX bufferu  
			//dalsi cmd

			//poslu retezec
			write (fd, "AT+CGSN=3\r\n", 11);
											printf("Sent command:");
											printf(ANSI_COLOR_YELLOW "\nAT+CGSN=3\n"ANSI_COLOR_RESET);  
											printf("\nReceived response:");          
											//prectu znak dokud nedojde K a enter
											//char byte_readed;
											bytes_readed = read(fd,&read_buffer,50);
											for(int j=0;j<bytes_readed; j++)
												printf(ANSI_COLOR_BLUE "%c"ANSI_COLOR_RESET,  read_buffer[j] );			
											printf("\n\n");	
			
											tcflush(fd, TCIFLUSH);   // zahodi stare data z RX bufferu  
			//dalsi cmd

			//poslu retezec
			write (fd, "AT+CSCON=1\r\n", 12);
											printf("Sent command:");
											printf(ANSI_COLOR_YELLOW "\nAT+CSCON=1\n"ANSI_COLOR_RESET);  
											printf("\nReceived response:");          
											//prectu znak dokud nedojde K a enter
											//char byte_readed;
											bytes_readed = read(fd,&read_buffer,50);
											for(int j=0;j<bytes_readed; j++)
												printf(ANSI_COLOR_BLUE "%c"ANSI_COLOR_RESET,  read_buffer[j] );			
											printf("\n\n");	
			
											tcflush(fd, TCIFLUSH);   /* zahodi stare data z RX bufferu  */
			//dalsi cmd
			//poslu retezec
			write (fd, "AT+CSCON?\r\n", 11);
											printf("Sent command:");
											printf(ANSI_COLOR_YELLOW "\nAT+CSCON?\n"ANSI_COLOR_RESET);  
											printf("\nReceived response:");          
											//prectu znak dokud nedojde K a enter
											//char byte_readed;
											bytes_readed = read(fd,&read_buffer,50);
											for(int j=0;j<bytes_readed; j++)
												printf(ANSI_COLOR_BLUE "%c"ANSI_COLOR_RESET,  read_buffer[j] );			
											printf("\n\n");	
			
											tcflush(fd, TCIFLUSH);   /* zahodi stare data z RX bufferu  */
			//dalsi cmd
			//poslu retezec
			write (fd, "AT+CFUN=1\r\n", 11);
											printf("Sent command:");
											printf(ANSI_COLOR_YELLOW "\nAT+CFUN=1\n"ANSI_COLOR_RESET);  
											printf("\nReceived response:");          
											//prectu znak dokud nedojde K a enter
											//char byte_readed;
											bytes_readed = read(fd,&read_buffer,50);
											for(int j=0;j<bytes_readed; j++)
												printf(ANSI_COLOR_BLUE "%c"ANSI_COLOR_RESET,  read_buffer[j] );			
											printf("\n\n");	
			
											tcflush(fd, TCIFLUSH);   /* zahodi stare data z RX bufferu  */
			//dalsi cmd
			//poslu retezec
			write (fd, "AT+CGDCONT=1,\"IP\"\r\n\r\n", 21);
											printf("Sent command:");
											printf(ANSI_COLOR_YELLOW "\nAT+CGDCONT=1,\"IP\"\n"ANSI_COLOR_RESET);  
											printf("\nReceived response:");          
											//prectu znak dokud nedojde K a enter
											//char byte_readed;
											bytes_readed = read(fd,&read_buffer,50);
											for(int j=0;j<bytes_readed; j++)
												printf(ANSI_COLOR_BLUE "%c"ANSI_COLOR_RESET,  read_buffer[j] );			
											printf("\n\n");	
			
											tcflush(fd, TCIFLUSH);   /* zahodi stare data z RX bufferu  */
			//dalsi cmd
			break;
			}



		case '3':
		{
			char read_buffer[50];   /* sem bude ukladat prijate data              */
				int  bytes_readed = 0;    /* pocet bytu ktere precte pri volani read() system callu */

				tcflush(fd, TCIFLUSH);   /* zahodi stare data z RX bufferu  */
				printf("Macro n.3 'AT+CGATT=1' just started.\n");
				while((c = getchar()) != '\n' && c != EOF)
				;//zahozeni entru
				
			
//poslu retezec
				write (fd, "AT+CGATT?\r\n", 12);  //409
				printf("\nSent command:\nAT+CGATT?");  
												printf("\nReceived response:");          
												//prectu znak dokud nedojde K a enter
												//char byte_readed;
												bytes_readed = read(fd,&read_buffer,50);
												for(int j=0;j<bytes_readed; j++)
													printf(ANSI_COLOR_BLUE "%c"ANSI_COLOR_RESET,  read_buffer[j] );		
												printf("\n\n");	
				
											 tcflush(fd, TCIFLUSH);   /* zahodi stare data z RX bufferu  */




			
			break;
		}
		
		case '4':
		{
				char read_buffer[50];   /* sem bude ukladat prijate data              */
				int  bytes_readed = 0;    /* pocet bytu ktere precte pri volani read() system callu */

				tcflush(fd, TCIFLUSH);   /* zahodi stare data z RX bufferu  */
				printf("Macro n.4 'eval' just started.\n");
				while((c = getchar()) != '\n' && c != EOF)
				;//zahozeni entru
				
				//poslu retezec
				write (fd, "AT+NBAND=20\r\n", 13);  //409
				printf("\nSent command:\nAT+NBAND=20");  
												printf("\nReceived response:");          
												//prectu znak dokud nedojde K a enter
												//char byte_readed;
												bytes_readed = read(fd,&read_buffer,5);
												for(int j=0;j<bytes_readed; j++)
													printf(ANSI_COLOR_BLUE "%c"ANSI_COLOR_RESET,  read_buffer[j] );		
												printf("\n\n");	
				
											 tcflush(fd, TCIFLUSH);   /* zahodi stare data z RX bufferu  */



//poslu retezec
				write (fd, "AT+CFUN=1\r\n", 11);  //409
				printf("\nSent command:\nAT+CFUN=1");  
												printf("\nReceived response:");          
												//prectu znak dokud nedojde K a enter
												//char byte_readed;
												bytes_readed = read(fd,&read_buffer,5);
												for(int j=0;j<bytes_readed; j++)
													printf(ANSI_COLOR_BLUE "%c"ANSI_COLOR_RESET,  read_buffer[j] );		
												printf("\n\n");	
				
											 tcflush(fd, TCIFLUSH);   /* zahodi stare data z RX bufferu  */

//poslu retezec
				write (fd, "AT+COPS=0\r\n", 11);  //409
				printf("\nSent command:\nAT+COPS=0");  
												printf("\nReceived response:");          
												//prectu znak dokud nedojde K a enter
												//char byte_readed;
												bytes_readed = read(fd,&read_buffer,50);
												for(int j=0;j<bytes_readed; j++)
													printf(ANSI_COLOR_BLUE "%c"ANSI_COLOR_RESET,  read_buffer[j] );		
												printf("\n\n");	
				
											 tcflush(fd, TCIFLUSH);   /* zahodi stare data z RX bufferu  */




//poslu retezec
				write (fd, "AT+CGDCONT=1,\"IP\",\"GPRST.INTERNET\"\r\n", 37);  //409
				printf("\nSent command:\nAT+CGDCONT=1,\"IP\",\"GPRST.INTERNET\"");  
												printf("\nReceived response:");          
												//prectu znak dokud nedojde K a enter
												//char byte_readed;
												bytes_readed = read(fd,&read_buffer,5);
												for(int j=0;j<bytes_readed; j++)
													printf(ANSI_COLOR_BLUE "%c"ANSI_COLOR_RESET,  read_buffer[j] );		
												printf("\n\n");	
				
											 tcflush(fd, TCIFLUSH);   /* zahodi stare data z RX bufferu  */

//poslu retezec
				write (fd, "AT+CGATT=1\r\n", 12);  //409
				printf("\nSent command:\nAT=CGATT=1");  
												printf("\nReceived response:");          
												//prectu znak dokud nedojde K a enter
												//char byte_readed;
												bytes_readed = read(fd,&read_buffer,5);
												for(int j=0;j<bytes_readed; j++)
													printf(ANSI_COLOR_BLUE "%c"ANSI_COLOR_RESET,  read_buffer[j] );		
												printf("\n\n");	
				
											 tcflush(fd, TCIFLUSH);   /* zahodi stare data z RX bufferu  */
//poslu retezec
				write (fd, "AT+CIMI\r\n", 9);  //409
				printf("\nSent command:\nAT+CIMI");  
												printf("\nReceived response:");          
												//prectu znak dokud nedojde K a enter
												//char byte_readed;
												bytes_readed = read(fd,&read_buffer,50);
												for(int j=0;j<bytes_readed; j++)
													printf(ANSI_COLOR_BLUE "%c"ANSI_COLOR_RESET,  read_buffer[j] );		
												printf("\n\n");	
				
											 tcflush(fd, TCIFLUSH);   /* zahodi stare data z RX bufferu  */

//poslu retezec
				write (fd, "AT+CSQ\r\n", 9);  //409
				printf("\nSent command:\nAT+CSQ");  
												printf("\nReceived response:");          
												//prectu znak dokud nedojde K a enter
												//char byte_readed;
												bytes_readed = read(fd,&read_buffer,50);
												for(int j=0;j<bytes_readed; j++)
													printf(ANSI_COLOR_BLUE "%c"ANSI_COLOR_RESET,  read_buffer[j] );		
												printf("\n\n");	
				
											 tcflush(fd, TCIFLUSH);   /* zahodi stare data z RX bufferu  */




			
			break;
}


		case '5': 
		{
				char read_buffer[50];   /* sem bude ukladat prijate data              */
				int  bytes_readed = 0;    /* pocet bytu ktere precte pri volani read() system callu */

				tcflush(fd, TCIFLUSH);   /* zahodi stare data z RX bufferu  */
				printf("Macro n.5 'DUMMY TRANSMITION' just started.\n");
				while((c = getchar()) != '\n' && c != EOF)
				;//zahozeni entru
				
				//poslu retezec
				write (fd, "AT+NRDTEST=196,3D0040021302BC0080950A30C0317C3200000000000000000000000000000000E80300008000000014000000000000000B000A00C80000000700020405010000170001000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000\r\n\r\n", 411);  //409
				printf("\nSent command:\nAT+NRDTEST=196,3D0040021302BC0080950A30C0317C32000000000000000000000\n00000000000E80300008000000014000000000000000B000A00C800000007000204050\n1000017000100000000000000000000000000000000000000000000000000000000000\n0000000000000000000000000000000000000000000000000000000000000000000000\n0000000000000000000000000000000000000000000000000000000000000000000000\n00000000000000000000000000000000000000000000000000000000000");  
												printf("\nReceived response:");          
												//prectu znak dokud nedojde K a enter
												//char byte_readed;
												bytes_readed = read(fd,&read_buffer,50);
												for(int j=0;j<bytes_readed; j++)
													printf(ANSI_COLOR_BLUE "%c"ANSI_COLOR_RESET,  read_buffer[j] );		
												printf("\n\n");	
				
											 tcflush(fd, TCIFLUSH);   /* zahodi stare data z RX bufferu  */
			
			break;
}

		case 'x':
			printf("\n\nKONEC..\n");
			quit = true;
			while((c = getchar()) != '\n' && c != EOF)
			/* discard */ ;
			break;

		default :
			printf("Neznamy znak\n");
			while((c = getchar()) != '\n' && c != EOF)
			/* discard */ ;

		}
	
	}			




	close(fd); // Close the serial port 

}
	
/*
		bytes_read = read(fd,&read_buffer,32); 

		printf("\n\n  Bytes Rxed - %d", bytes_read); // Print the number of bytes read 
		printf("\n\n  ");




		for(i=0;i<bytes_read;i++)	 // printing only the received characters
		    printf("%c",read_buffer[i]);

*/

