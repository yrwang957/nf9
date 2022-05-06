#include <stdio.h>
#include <stdlib.h>

#include "nf9headers.h"
#include "def.h"

char* _fs_id_to_string(uint16_t fs_id)
{
    switch(fs_id)
    {
    case 0:
        return "Template";
    case 1:
        return "OptionTemplate";
    default:
        return "Data";
    }

    return "Unknown";
}

int process_flowSet(FlowSet_header* fs_header)
{
    uint16_t fs_id = 0;
    uint16_t length = 0;
    int returned_count = 0;

    fs_id = ntohs(fs_header->flowSetId);
    length = ntohs(fs_header->length);

    printf("  id  %hu (%s)\n", fs_id, _fs_id_to_string(fs_id));
    printf("  len %hu\n", length);

    switch(fs_id)
    {
    // Template sets
    case TEMPLATE_FLOWSET:
        returned_count = templateFlowSet(fs_header);
        break;

    // Option Template sets
    case OPTION_TEMPLATE:
        // ignore for now
        // returned_count = optionTemplate(fs_header);
        break;

    // Data_header sets
    default:
        returned_count = data(fs_header);
        break;
    }

    return returned_count;
}

// +-----------------+
// | Template sets   |
// +-----------------+
int templateFlowSet(FlowSet_header* p)
{
    int i = 0;
    uint16_t length = ntohs(p->length);
    uint16_t pLength = 4;

    printf("    Template:\n");
    printf("    length %d\n\n", length);

    TemplateFlowSet_header* t = (TemplateFlowSet_header*)((char*)p + sizeof(FlowSet_header));

    //Unpack t
    for(i = 0 ; ; ++i)
    {
        uint16_t templateId = ntohs(t->templateId);
        uint16_t fieldCount = ntohs(t->fieldCount);
        uint16_t tLength = 4 + (fieldCount << 2);

        if(templateId == 0)
            continue;

        printf("      %03d : tId %d, tLength %d, FieldCount %d\n", i, templateId, tLength, fieldCount);

        // if((ret = putBuf(BUF_TEMPLATE, tLength, templateId, (void*)t)) != SUCCESS)
        // {
        //     printf("    ret value %d\n", ret);
        // }
        // printf("\n");

        t = (TemplateFlowSet_header*)((char*)t + tLength);
        pLength += tLength;
        if((pLength >= length) || ((length - pLength) <= 4))
        {
            break;
        }
    }
    printf("\n");

    return 1;
}

// +-----------------+
// | Data_header sets       |
// +-----------------+
int data(FlowSet_header* p)
{
    Data_header* d = (Data_header*)p;
    uint16_t fSId = ntohs(d->flowSetId);
    uint16_t length = ntohs(d->length);
    uint16_t padding = (length - 4) & 0x03;

    printf("    Data_header:\n");
    printf("    fSId %d, Length %d, Padding %d\n", fSId, length, padding);

    //Data_header no need unpack, include paddding
    // if((ret = putBuf(BUF_DATA, length, fSId, (void*)d)) != SUCCESS)
    // {
    //     printf("    ret value %d\n", ret);
    // }
    // printf("\n");

    return 1;
}

// +----------------------+
// | Option Template sets |
// +----------------------+
int optionTemplate(FlowSet_header* p)
{
    // don't care for now, only record templ_id

    return 1;

    /*
    int i = 0;
    int length = ntohs(p->length);
    int pLength = 4;
    int padding= (length - 4) & 0x03;

    printf("    OptionsTemplate:\n");
    printf("    length %d, padding %d\n", length, padding);

    OptionsTemplate* t = (OptionsTemplate*)(p + sizeof(FlowSet_header));

    //Unpack ot
    for(i = 0; ; ++i)
    {
        int templateId = ntohs(t->templateId);
        int scopeLength = ntohs(t->optionScopeLength);
        int optionsLength = ntohs(t->optionsLength);
        int otLength = optionsLength;

        printf("      %03d : tId %d, oLength %d, counts %d|%d\n", i, templateId, optionsLength, scopeLength >> 2, optionsLength >> 2);

        // if((ret = putBuf(BUF_OTEMPLATE, optionsLength, templateId, (void*)t)) != SUCCESS)
        // {
        //     printf("    ret value %d\n", ret);
        // }
        // printf("\n");

        t = (OptionsTemplate*)((char*)t + otLength);
        pLength += otLength;
        if((pLength >= length) || ((length - pLength) <= 4))
        {
            break;
        }
    }
    printf("\n");
    */

    return 1;
}
