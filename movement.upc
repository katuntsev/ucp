#include <upc_relaxed.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#define MAX 10000000
#define CONFIG "config"
#define OUTPUT_FILENAME "output"
#define MAX_STEP_COUNT 100
#define ASD THREADS

/*****************************************************************************/
/* x = vx*t + x0;                                                            */
/* y = vy*t + y0;                                                            */
/* z = vz*t + z0;                                                            */
/* v = (vx^2 + vy^2 + vz^2)^(1/2)                                            */
/* v3 = (m1*v1 + m2*v2)/(m1 + m2)                                            */
/*****************************************************************************/
shared int will_clash = 0;
shared char out_array[MAX_STEP_COUNT][64*THREADS];
shared int x1[MAX_STEP_COUNT*THREADS];
shared int y1[MAX_STEP_COUNT*THREADS];
shared int z1[MAX_STEP_COUNT*THREADS];
shared int x2[MAX_STEP_COUNT*THREADS];
shared int y2[MAX_STEP_COUNT*THREADS];
shared int z2[MAX_STEP_COUNT*THREADS];

int main(int argc, char* argv[])
{
	int x10, y10, z10;
	int vx1, vy1, vz1;
	
	int x20, y20, z20;
	int vx2, vy2, vz2;
	int m1, m2;
	int i = 0;

	
	FILE * config;
	config = fopen(CONFIG, "r");
	if (NULL == config)
	{
		printf("can't open config file\n");
		return -1;
	}
	FILE * out;
	if (MYTHREAD == 0)
	{
		out = fopen(OUTPUT_FILENAME, "w");
		if (NULL == out)
		{
			printf("can't open output file\n");
			return -1;
		}
	}
	
	
	fscanf(config,
	       "x0=%d y0=%d z0=%d vx=%d vy=%d vz=%d m=%d\n"
	       "x0=%d y0=%d z0=%d vx=%d vy=%d vz=%d m=%d",
	       &x10, &y10, &z10, &vx1, &vy1, &vz1, &m1,
	       &x20, &y20, &z20, &vx2, &vy2, &vz2, &m2);
	
	upc_forall (i = 0; i < MAX_STEP_COUNT * 2; i++; (i*THREADS)/(MAX_STEP_COUNT * 2))
	{
		if (i % 2 == 0)
		{
			x1[i/2] = vx1*i + x10;
			y1[i/2] = vy1*i + y10;
			z1[i/2] = vz1*i + z10;
		}
		else
		{
			x2[i/2] = vx2*(i-1) + x20;
			y2[i/2] = vy2*(i-1) + y20;
			z2[i/2] = vz2*(i-1) + z20;
		}
		
		if (x1[i/2] == x2[i/2] && y1[i/2] == y2[i/2] && z1[i/2] == z2[i/2])
			will_clash = i;
	}
	usleep(1);
	if (!will_clash)
	{
		printf("will not clash\n");
		goto EXIT_LABEL;
	}
	else
	{
		printf("will clash %d\n", will_clash);
	}
	return 0;
	
	int x3, y3, z3;
	int x30, y30, z30;
	int vx3, vy3, vz3;
	
	for (i = 0; i <= will_clash; i++)
	{
		if (i % 2 == 0)
		{
			x1 = vx1*i + x10;
			y1 = vy1*i + y10;
			z1 = vz1*i + z10;
		}
		else
		{
			x2 = vx2*(i-1) + x20;
			y2 = vy2*(i-1) + y20;
			z2 = vz2*(i-1) + z20;
			sprintf(out_array[i/2], "%d %d %d    %d %d %d\n",
			        x1, y1, z1, x2, y2, z2);
		}
	}
	
	if (x1 != x2 || y1 != y2 || z1 != z2)
	{
		printf("error fail warning emergency\n");
		goto EXIT_LABEL;
	}
	
	x30 = x1; y30 = y1; z30 = z1;
	vx3 = (m1*vx1 + m2*vx2)/(m1 + m2);
	vy3 = (m1*vy1 + m2*vy2)/(m1 + m2);
	vz3 = (m1*vz1 + m2*vz2)/(m1 + m2);
	
	
	for (i = 1; i < (MAX_STEP_COUNT * 2 - will_clash) / 2; i++)
	{
		x3 = vx3*i + x30;
		y3 = vy3*i + y30;
		z3 = vz3*i + z30;
		//sprintf(out_array[will_clash/2 + i], "%d %d %d\n", x3, y3, z3);
	}
	
	if (MYTHREAD == 0)
	{
		for (i = 0; i < MAX_STEP_COUNT; i++)
		{
			fprintf(out, "%s", out_array[i]);
		}
	}
EXIT_LABEL:
	fclose(config);
	if (MYTHREAD == 0)
		fclose(out);
	
	return 0;	
}
