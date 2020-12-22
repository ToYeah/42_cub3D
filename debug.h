#ifndef DEBUG_H
# define DEBUG_H

# include <stdio.h>

# define DEBUGV(v_fmt, v) \
    printf(#v ": " v_fmt "\t(file \"%s\", line %d, in %s)\n", \
        v, __FILE__, __LINE__, __FUNCTION__);

# define DC(v) DEBUGV("%c", v);
# define DS(v) DEBUGV("%s", v);
# define DI(v) DEBUGV("%d", v);
# define DF(v) DEBUGV("%f", v);
# define DD(v) DEBUGV("%lf", v);
# define DX(v) DEBUGV("%x", v);
# define DL(v) DEBUGV("%ld", v);

# define DEBUGF(fmt, ...) \
    printf(fmt "\t(file \"%s\", line %d, in %s)\n", \
        __VA_ARGS__, __FILE__, __LINE__, __FUNCTION__);

#endif