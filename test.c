#include "minilibx-linux/mlx.h"

#include <stdio.h>
#include <errno.h>
#include <string.h>

int     main(void)
{
    printf("%s",strerror(errno));
}