#include <stdio.h>
#include <stdlib.h>

#include "nf9r.h"

int templateFlowSet(FlowSetHeader* fs)
{
    TemplateFlowSet* t = (TemplateFlowSet*)fs;

    printf("=TemplateFlowSet=\n");
    printf("TemplateId %u, FieldCount %u\n\n", t->templateId, t->fieldCount);

    return 0;
}

int optionTemplate(FlowSetHeader* fs)
{
    OptionsTemplate* ot = (OptionsTemplate*)fs;

    printf("=OptionTemplate=\n");
    printf("TemplateId %u, ScopeLength %u, Length %u\n\n", ot->templateId, ot->optionScopeLength, ot->optionsLength);

    return 0;
}

int data(FlowSetHeader* fs)
{
    Data* d = (Data*)fs;

    printf("=Data=\n");
    printf("=TemplateId %u\n\n", d->flowSetId);

    return 0;
}
