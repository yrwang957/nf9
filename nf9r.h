#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define TEMPLATE_FLOWSET   0
#define OPTION_TEMPLATE    1

typedef struct _NF9Header
{
    uint16_t version;
    uint16_t count;
    uint32_t systemUpTime;
    uint32_t unixSeconds;
    uint32_t packetSeq;
    uint32_t SourceId;
}NF9Header;

typedef struct _FlowSetHeader
{
    uint16_t flowSetId;
    uint16_t length;
}FlowSetHeader;

typedef struct _Data
{
    uint16_t flowSetId;
    uint16_t length;
    void*     records;
}Data;

typedef struct _Field
{
    uint16_t type;
    uint16_t length;
}Field;

typedef struct _TemplateFlowSet
{
    uint16_t templateId;
    uint16_t fieldCount;
    Field     fields;
}TemplateFlowSet;

typedef struct _OptionsTemplate
{
    uint16_t templateId;
    uint16_t optionScopeLength;
    uint16_t optionsLength;
    Field     OptionFields;
    void*     fields;
}OptionsTemplate;

int templateFlowSet(FlowSetHeader*);
int optionTemplate(FlowSetHeader*);
int data(FlowSetHeader*);
