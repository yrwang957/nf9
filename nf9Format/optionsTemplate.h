#include <stdio.h>
#include <stdlib.h>

struct OptionScope
{
    u_int16_t type;
    u_int16_t length;
};

struct OptionField
{
    u_int16_t type;
    u_int16_t length;
};

struct Nf9OptionsTemplate
{
    u_int16_t flowSetId;
    u_int16_t length;
    u_int16_t templateId;
    u_int16_t optionScopeLength;
    u_int16_t optionsLength;
};
