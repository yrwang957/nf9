#ifndef NF9HEADERS_H
#define NF9HEADERS_H

#include <netinet/in.h>

#define TEMPLATE_FLOWSET   0
#define OPTION_TEMPLATE    1

typedef struct
{
    uint16_t version;
    uint16_t count;
    uint32_t systemUpTime;
    uint32_t unixSeconds;
    uint32_t packetSeq;
    uint32_t sourceId;
}NetFlow9_header;

typedef struct
{
    uint16_t flowSetId;
    uint16_t length;
}FlowSet_header;

typedef struct
{
    uint16_t templateId;
    uint16_t fieldCount;
}TemplateFlowSet_header;

typedef struct
{
    uint16_t id;
    uint16_t length;
}Data;

typedef struct
{
    uint16_t templateId;
    uint16_t optionScopeLength;
    uint16_t optionsLength;
}OptionsTemplate_header;

typedef struct
{
    uint16_t type;
    uint16_t length;
}Field;

int templateFlowSet(FlowSet_header* fs_header);
int optionTemplate(FlowSet_header* fs_header);
int data(FlowSet_header* fs_header);

int process_flowSet(FlowSet_header* fs_header);

#endif // NF9HEADERS_H
