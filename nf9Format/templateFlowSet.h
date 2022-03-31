#include <stdio.h>
#include <stdlib.h>

struct Field
{
    u_int16_t type;
    u_int16_t length;
};

struct Nf9TemplateFlowSet
{
    u_int16_t flowSetId;
    u_int16_t length;
    u_int16_t templateId;
    u_int16_t fieldCount;
};
