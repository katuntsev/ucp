#include <stdio.h>
#include <unistd.h>

#define CONFIG "config"
#define OUTPUT_FILENAME "output"
#define MAX_STEP_COUNT 100

/*****************************************************************************/
/* x = vx*t + x0;                                                            */
/* y = vy*t + y0;                                                            */
/* z = vz*t + z0;                                                            */
/* v = (vx^2 + vy^2 + vz^2)^(1/2)                                            */
/*****************************************************************************/

int main(int argc, char* argv[])
{
	int will_clash = 0;
	int x1, y1, z1;
	int x10, y10, z10;
	int vx1, vy1, vz1;
	int x2, y2, z2;
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
	out = fopen(OUTPUT_FILENAME, "w");
	if (NULL == out)
	{
		printf("can't open output file\n");
		return -1;
	}
	
	
	fscanf(config,
	       "x0=%d y0=%d z0=%d vx0=%d vy0=%d vz0=%d m=%d\n"
	       "x0=%d y0=%d z0=%d vx0=%d vy0=%d vz0=%d m=%d",
	       &x10, &y10, &z10, &vx1, &vy1, &vz1, &m1,
	       &x20, &y20, &z20, &vx2, &vy2, &vz2, &m2);
	
	while(i < MAX_STEP_COUNT)
	{
		x1 = vx1*i + x10;
		y1 = vy1*i + y10;
		z1 = vz1*i + z10;
		
		x2 = vx2*i + x20;
		y2 = vy2*i + y20;
		z2 = vz2*i + z20;
		
		if (x1 == x2 && y1 == y2 && z1 ==z2)
			will_clash = 1;
		
		i++;
	}
	
	if (!will_clash)
	{
		printf("will not clash\n");
		goto EXIT_LABEL;
	}
	else
	{
		printf("will clash\n");
	}

EXIT_LABEL:
	fclose(config);
	fclose(out);
	
	return 0;	
}
