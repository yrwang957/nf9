#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>

#define TEMPLATE_FLOWSET   0
#define OPTION_TEMPLATE    1

typedef struct _NF9Header
{
    u_int16_t version;
    u_int16_t count;
    u_int32_t systemUpTime;
    u_int32_t unixSeconds;
    u_int32_t packetSeq;
    u_int32_t SourceId;
}NF9Header;

typedef struct _FlowSetHeader
{
    u_int16_t flowSetId;
    u_int16_t length;
}FlowSetHeader;

typedef struct _Data
{
    u_int16_t flowSetId;
    u_int16_t length;
    void*     records;
}Data;

typedef struct _Field
{
    u_int16_t type;
    u_int16_t length;
}Field;

typedef struct _TemplateFlowSet
{
    u_int16_t templateId;
    u_int16_t fieldCount;
    Field     fields;
}TemplateFlowSet;

typedef struct _OptionsTemplate
{
    u_int16_t templateId;
    u_int16_t optionScopeLength;
    u_int16_t optionsLength;
    Field     OptionFields;
    void*     fields;
}OptionsTemplate;

int templateFlowSet(FlowSetHeader*);
int optionTemplate(FlowSetHeader*);
int data(FlowSetHeader*);
