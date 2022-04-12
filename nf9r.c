#include <stdio.h>
#include <stdlib.h>

#include "nf9r.h"
#include "buffer.h"

// +-----------------+
// | Template sets   |
// +-----------------+
int templateFlowSet(FlowSetHeader* pFs)
{
    int i = 0;
    int ret = 0;
    int length = ntohs(pFs->length);
    int pLength = 4;

    printf("    Template:\n");
    printf("    length %d\n\n", length);

    TemplateFlowSet* t = (TemplateFlowSet*)((char*)pFs + sizeof(FlowSetHeader));

    //Unpack t
    for(i = 0 ; ; ++i)
    {
        int templateId = ntohs(t->templateId);
        int fieldCount = ntohs(t->fieldCount);
        int tLength = 4 + (fieldCount << 2);

        if(templateId == 0)
            continue;

        printf("      %03d : tId %d, tLength %d, FieldCount %d\n", i, templateId, tLength, fieldCount);

        if((ret = putBuf(BUF_TEMPLATE, tLength, templateId, (void*)t)) != SUCCESS)
        {
            printf("    ret value %d\n", ret);
        }
        printf("\n");

        t = (TemplateFlowSet*)((char*)t + tLength);
        pLength += tLength;
        if((pLength >= length) || ((length - pLength) <= 4))
        {
            break;
        }
    }
    printf("\n");

    return 0;
}

// +-----------------+
// | Data sets       |
// +-----------------+
int data(FlowSetHeader* pFs)
{
    int ret = 0;
    Data* d = (Data*)pFs;
    int flowSetId = ntohs(d->flowSetId);
    int length = ntohs(d->length);
    int padding = (length - 4) & 0x03;

    printf("    Data:\n");
    printf("    flowSetId %d, Length %d, Padding %d\n", flowSetId, length, padding);

    //Data no need unpack, include paddding
    if((ret = putBuf(BUF_DATA, length, flowSetId, (void*)d)) != SUCCESS)
    {
        printf("    ret value %d\n", ret);
    }
    printf("\n");

    return 0;
}

// +----------------------+
// | Option Template sets |
// +----------------------+
int optionTemplate(FlowSetHeader* pFs)
{
    int i = 0;
    int ret = 0;
    int length = ntohs(pFs->length);
    int pLength = 4;
    int padding= (length - 4) & 0x03;

    printf("    OptionsTemplate:\n");
    printf("    length %d, padding %d\n", length, padding);

    OptionsTemplate* t = (OptionsTemplate*)(pFs + sizeof(FlowSetHeader));

    //Unpack ot
    for(i = 0; ; ++i)
    {
        int templateId = ntohs(t->templateId);
        int scopeLength = ntohs(t->optionScopeLength);
        int optionsLength = ntohs(t->optionsLength);
        int otLength = optionsLength;

        printf("      %03d : tId %d, oLength %d, counts %d|%d\n", i, templateId, optionsLength, scopeLength >> 2, optionsLength >> 2);

        if((ret = putBuf(BUF_OTEMPLATE, optionsLength, templateId, (void*)t)) != SUCCESS)
        {
            printf("    ret value %d\n", ret);
        }
        printf("\n");

        t = (OptionsTemplate*)((char*)t + otLength);
        pLength += otLength;
        if((pLength >= length) || ((length - pLength) <= 4))
        {
            break;
        }
    }
    printf("\n");

    return 0;
}
