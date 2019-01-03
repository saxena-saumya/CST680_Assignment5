/* Author: Saumya Saxena
Assignment 5 - Computing in the Small
Date: June 13, 2018
*/
#include <u.h>
#include <libc.h>

void 
wait480(vlong start)
{
	for(int i = 0; i < 250; i++){
		sleep(0);
	}
	vlong end = nsec();
	vlong diff = end-start;
}

void 
wait60(vlong start)
{
	for(int i = 0; i < 29; i++){
		sleep(0);
	}
	vlong end = nsec();
	vlong diff = end-start;
}

void
receive(int fd)
{
	vlong start, gvals;
	char buf[16];
	int j, arr[16];

	start = nsec();
	for(j= 0; j < 8; j++){
			fprint(fd, "function 4 pulse");
			read(fd, buf, 8);
			buf[8] = 0;
			gvals = strtoull(buf, nil, 8);
			arr[j] = (gvals & (1 << 9)) >> 9;
			wait60(start);
		}

	for(j = 7; j >= 0; j--){
			print("%d", arr[j]);
		}
	print("\n");
}

void
sendcommand(unint x, int fd)
{
	int i;
	for(i = 0; i < 8; i++)
		{
			x = (x & 128);
			x = (x << 1) & 255;
			if(x == 1)
			{
				fprint(fd, "function 4 pulse");
				wait60(start);
			}
			else
			{
				fprint(fd, "function 4 out");
				wait60(start);
				fprint(fd, "function 4 in");
			}
		}
}

void
main()
{
	int fd, j;

	fd = open("/dev/gpio", ORDWR);
	
	if(fd < 0){
		bind("#G", "/dev", MAFTER);
		fd = open("/dev/gpio", ORDWR);
		if(fd < 0)
			print("gpio open error: %r\n");
	}

	fprint(fd, "set 4 0");

	while(1){

		vlong start = nsec();
		fprint(fd, "function 17 out"); //Reset
		wait480(start);
		fprint(fd, "function 17 in");
		wait480(start);
		sendcommand(11001100, fd); //Skip ROM
		sendcommand(01000100, fd); //Command

		wait480(start);
		fprint(fd, "function 17 out"); //Reset
		wait480(start);
		fprint(fd, "function 17 in");
		wait480(start);
		sendcommand(11001100, fd); //Skip ROM
		sendcommand(10111110, fd); //Read

		for(j = 0; j<9; j++)
		{
			receive(fd);
		}
	}
} 
