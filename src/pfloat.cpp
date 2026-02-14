#include <cmath>
#include "types.h"
#include "pfloat.h"

FP COS[360];
FP SIN[360];

void populateTables(void)
{
    u16 i = 0;
    while (i < 360)
    {
        COS[i] = FLOAT_TO_FP(cos(i * 0.0174533));
        SIN[i] = FLOAT_TO_FP(sin(i * 0.0174533));
        i++;
    }
}
