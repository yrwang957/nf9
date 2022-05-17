#include <stdio.h>
#include <stdlib.h>

#include "nf9headers.h"
#include "def.h"

char* _fs_id_string(uint16_t fs_id)
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
    int ret_processed = 0;

    fs_id = ntohs(fs_header->flowSetId);
    length = ntohs(fs_header->length);

    printf("  id  %hu (%s)\n", fs_id, _fs_id_string(fs_id));
    printf("  len %hu\n", length);

    switch(fs_id)
    {
    case TEMPLATE_FLOWSET:
        ret_processed = templateFlowSet(fs_header);
        break;

    case OPTION_TEMPLATE:
        // TODO:
        // ret_processed = optionTemplate(fs_header);
        break;

    // Default is Data
    default:
        ret_processed = data(fs_header);
        break;
    }

    return ret_processed;
}

// +-----------------+
// | Template sets   |
// +-----------------+
int templateFlowSet(FlowSet_header* header)
{
    int i = 0;
    int processed_flowset = 0;
    uint16_t length = ntohs(header->length);
    uint16_t processed_bytes = 4;

    printf("    Template:\n");
    printf("    length %d\n", length);

    TemplateFlowSet_header* templ_header = (TemplateFlowSet_header*)((uint8_t*)header + sizeof(FlowSet_header));

    //Unpack templ_header
    for(i = 0 ; ; ++i)
    {
        uint16_t templateId = ntohs(templ_header->templateId);
        uint16_t fieldCount = ntohs(templ_header->fieldCount);
        uint16_t templateLen = 4 + (fieldCount << 2);

        if(templateId == 0)
            continue;

        printf("      %d: id %d, FieldCount %d (len %d)\n", i + 1, templateId, fieldCount, templateLen);

int prinnttempl = 0;
if(prinnttempl)
{
        uint8_t* ptr = ((uint8_t*)templ_header) + 4;
        for(int fieldIdx = 0; fieldIdx < fieldCount; ++fieldIdx)
        {
            uint16_t field_value = ptr[0] << 8 | ptr[1];
            uint16_t field_length = ptr[2] << 8 | ptr[3];

            printf("       %2hu: v %-3hu, l %-3hu\n", fieldIdx + 1, field_value, field_length);
            ptr += 4;
        }
}

        // if((ret = putBuf(BUF_TEMPLATE, templateLen, templateId, (void*)templ_header)) != SUCCESS)
        // {
        //     printf("    ret value %d\n", ret);
        // }
        // printf("\n");

        // Next template
        ++processed_flowset;
        templ_header = (TemplateFlowSet_header*)(((uint8_t*)templ_header) + templateLen);
        processed_bytes += templateLen;
        if((processed_bytes >= length) || ((length - processed_bytes) <= 4))
        {
            break;
        }
    }

    return processed_flowset;
}

// +-----------------+
// | Data sets       |
// +-----------------+
int data(FlowSet_header* p)
{
    Data* d = (Data*)p;
    uint16_t fSId = ntohs(d->id);
    uint16_t length = ntohs(d->length);
    uint16_t padding = (length - 4) & 0x03;

    printf("    Data:\n");
    printf("    fSId %d, Length %d, Padding %d\n", fSId, length, padding);

    //Data no need unpack, include paddding
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
