#ifndef NF9R_H
#define NF9R_H

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
    uint32_t sourceId;
}NF9Header;

typedef struct _FlowSetHeader
{
    uint16_t flowSetId;
    uint16_t length;
}FlowSetHeader;

typedef struct _TemplateFlowSet
{
    uint16_t templateId;
    uint16_t fieldCount;
}TemplateFlowSet;

typedef struct _Data
{
    uint16_t flowSetId;
    uint16_t length;
}Data;

typedef struct _OptionsTemplate
{
    uint16_t templateId;
    uint16_t optionScopeLength;
    uint16_t optionsLength;
}OptionsTemplate;

typedef struct _Field
{
    uint16_t type;
    uint16_t length;
}Field;

int templateFlowSet(FlowSetHeader*);
int optionTemplate(FlowSetHeader*);
int data(FlowSetHeader*);

#endif // NF9R_H
