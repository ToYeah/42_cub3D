#include "minilibx-linux/mlx.h"

#include <stdio.h>
#include <errno.h>
#include <string.h>




int     main(int argc, char **argv)
{
	printf("\n%:vd", 1);
	printf("\n%;vd", 1);
	printf("\n%:qd", 1);
	printf("\n%:kd", 1);
	printf("\n%!vd", 1);
	printf("\n%lvd", 1);
}