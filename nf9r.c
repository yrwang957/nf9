#include <stdio.h>
#include <stdlib.h>

#include "def.h"
#include "nf9r.h"

// +-----------------+
// | Template sets   |
// +-----------------+
int templateFlowSet(FlowSetHeader* p)
{
    int i = 0;
    uint16_t length = ntohs(p->length);
    uint16_t pLength = 4;

    printf("    Template:\n");
    printf("    length %d\n\n", length);

    TemplateFlowSet* t = (TemplateFlowSet*)((char*)p + sizeof(FlowSetHeader));

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

        t = (TemplateFlowSet*)((char*)t + tLength);
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
// | Data sets       |
// +-----------------+
int data(FlowSetHeader* p)
{
    Data* d = (Data*)p;
    uint16_t flowSetId = ntohs(d->flowSetId);
    uint16_t length = ntohs(d->length);
    uint16_t padding = (length - 4) & 0x03;

    printf("    Data:\n");
    printf("    flowSetId %d, Length %d, Padding %d\n", flowSetId, length, padding);

    //Data no need unpack, include paddding
    // if((ret = putBuf(BUF_DATA, length, flowSetId, (void*)d)) != SUCCESS)
    // {
    //     printf("    ret value %d\n", ret);
    // }
    // printf("\n");

    return 1;
}

// +----------------------+
// | Option Template sets |
// +----------------------+
int optionTemplate(FlowSetHeader* p)
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

    OptionsTemplate* t = (OptionsTemplate*)(p + sizeof(FlowSetHeader));

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
