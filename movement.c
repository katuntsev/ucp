#include <stdio.h>

#define CONFIG "config"

/*****************************************************************************/
/* x = vx0*t + (gx*t^2)/2 + x0;                                              */
/* vx = v0x + gx*t;                                                          */
/* y = vy0*t + (gy*t^2)/2 + y0;                                              */
/* vy = v0y + gy*t;                                                          */
/* z = vz0*t + (gz*t^2)/2 + z0;                                              */
/* vz = v0z + gz*t;                                                          */
/*****************************************************************************/

int main(int argc, char* argv[])
{
	int x0, y0, z0;
	int vx0, vy0, vz0;
	int gx, gy, gz;
	
	FILE * file;
	file = fopen(CONFIG, "r");
	if (NULL == file)
	{
		printf("can't open config file");
		return -1;
	}
	
	fscanf(file,
	       "x0=%d y0=%d z0=%d vx0=%d vy0=%d vz0=%d gx=%d gy=%d gz=%d",
	       &x0, &y0, &z0, &vx0, &vy0, &vz0, &gx, &gy, &gz);
	              
	
	
	fclose(file);
	
	return 0;	
}
