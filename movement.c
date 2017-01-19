#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

#define CONFIG "config"
#define OUTPUT_FILENAME "output2"
#define MAX_STEP_COUNT 232720

/*****************************************************************************/
/* x = vx*t + x0;                                                            */
/* y = vy*t + y0;                                                            */
/* z = vz*t + z0;                                                            */
/* v = (vx^2 + vy^2 + vz^2)^(1/2)                                            */
/* v3 = (m1*v1 + m2*v2)/(m1 + m2)                                            */
/*****************************************************************************/

int main(int argc, char* argv[])
{
	clock_t start, end;
	start = clock();
	int will_clash = 0;
	int x30, y30, z30;
	int vx3, vy3, vz3;
	int x1[MAX_STEP_COUNT];
	int y1[MAX_STEP_COUNT];
	int z1[MAX_STEP_COUNT];
	int x2[MAX_STEP_COUNT];
	int y2[MAX_STEP_COUNT];
	int z2[MAX_STEP_COUNT];
	int x3[MAX_STEP_COUNT];
	int y3[MAX_STEP_COUNT];
	int z3[MAX_STEP_COUNT];
	int x10, y10, z10;
	int vx1, vy1, vz1;
	int x20, y20, z20;
	int vx2, vy2, vz2;
	int m1, m2;
	long i = 0;

	
	FILE * config;
	config = fopen(CONFIG, "r");
	if (NULL == config)
	{
		printf("can't open config file\n");
		return -1;
	}
	FILE * out;
	
	out = fopen(OUTPUT_FILENAME, "w");
	if (NULL == out)
	{
		printf("can't open output file\n");
		return -1;
	}	
	
	fscanf(config,
	       "x0=%d y0=%d z0=%d vx=%d vy=%d vz=%d m=%d\n"
	       "x0=%d y0=%d z0=%d vx=%d vy=%d vz=%d m=%d",
	       &x10, &y10, &z10, &vx1, &vy1, &vz1, &m1,
	       &x20, &y20, &z20, &vx2, &vy2, &vz2, &m2);
	
	for (i = 0; i < MAX_STEP_COUNT * 2; i++)
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

	if (!will_clash)
	{
		printf("will not clash\n");
		goto EXIT_LABEL;
	}
	else
	{
		printf("will clash %d\n", will_clash);
	}
	
	
	for (i = 0; i < will_clash; i++)
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
	}
	if (x1[i/2] == x2[i/2] ||
	    y1[i/2] == y2[i/2] || 
	    z1[i/2] == z2[i/2])
	{
		x30 = x1[i/2]; y30 = y1[i/2]; z30 = z1[i/2];
		vx3 = (m1*vx1 + m2*vx2)/(m1 + m2);
		vy3 = (m1*vy1 + m2*vy2)/(m1 + m2);
		vz3 = (m1*vz1 + m2*vz2)/(m1 + m2);
	}
	
	for (i = 1; i < (MAX_STEP_COUNT * 2 - will_clash) / 2; i++)
	{
		x3[i] = vx3*i + x30;
		y3[i] = vy3*i + y30;
		z3[i] = vz3*i + z30;
	}
	end = clock();
	double cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
	printf("time1 = %f\n", cpu_time_used);

	for (i = 0; i < MAX_STEP_COUNT; i++)
	{
		if (i <= will_clash/2)
			fprintf(out, "%d %d %d    %d %d %d\n",
			x1[i], y1[i], z1[i], x2[i], y2[i], z2[i]);
		else
			fprintf(out, "%d %d %d\n", x3[i-will_clash/2],
			              y3[i-will_clash/2], z3[i-will_clash/2]);
	}

EXIT_LABEL:
	fclose(config);
	fclose(out);

	end = clock();
	cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
	printf("time2 = %f\n", cpu_time_used);
	return 0;	
}
