// NEWS GENERATOR - LETS YOU CHANGE ARTICLE HEADLINE, DATE, AND SUMMARY

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE 1024
#define MAX_SUMMARY 4096

void replace_placeholder(char *line, const char *placeholder, const char *text)
{
    char buffer[MAX_LINE];
    char *pos = strstr(line, placeholder);
    if (pos != NULL)
    {
        *pos = '\0';
        snprintf(buffer, MAX_LINE, "%s%s%s", line, text, pos + strlen(placeholder));
        strcpy(line, buffer);
    }
}

void read_summary(char *buffer, size_t size)
{
    printf("(Enter summary, end with an empty line):\n");
    buffer[0] = '\0';
    char temp[512];
    while (fgets(temp, sizeof(temp), stdin))
    {
        if (strcmp(temp, "\n") == 0)
            break;
        if (strlen(buffer) + strlen(temp) < size - 1)
            strcat(buffer, temp);
        else
            break;
    }
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n')
        buffer[len - 1] = '\0';
}

int main()
{
    FILE *template = fopen("template.html", "r");
    if (!template)
    {
        printf("Error: template.html not found.\n");
        return 1;
    }

    FILE *output = fopen("index.html", "w");
    if (!output)
    {
        printf("Error: cannot create index.html\n");
        fclose(template);
        return 1;
    }

    char line[MAX_LINE];

    const char *placeholders[] = {
        "AI1_TITLE", "AI1_DATE", "AI1_SUMMARY",
        "AI2_TITLE", "AI2_DATE", "AI2_SUMMARY",
        "GADGET1_TITLE", "GADGET1_DATE", "GADGET1_SUMMARY",
        "GADGET2_TITLE", "GADGET2_DATE", "GADGET2_SUMMARY",
        "HW1_TITLE", "HW1_DATE", "HW1_SUMMARY",
        "HW2_TITLE", "HW2_DATE", "HW2_SUMMARY",
        "GEN1_TITLE", "GEN1_DATE", "GEN1_SUMMARY",
        "GEN2_TITLE", "GEN2_DATE", "GEN2_SUMMARY"};

    char inputs[24][MAX_SUMMARY];

    for (int i = 0; i < 24; i += 3)
    {
        printf("Enter %s: ", placeholders[i]);
        fgets(inputs[i], 256, stdin);
        inputs[i][strcspn(inputs[i], "\n")] = 0;

        printf("Enter %s: ", placeholders[i + 1]);
        fgets(inputs[i + 1], 256, stdin);
        inputs[i + 1][strcspn(inputs[i + 1], "\n")] = 0;

        printf("Enter %s:\n", placeholders[i + 2]);
        read_summary(inputs[i + 2], MAX_SUMMARY);
    }

    while (fgets(line, MAX_LINE, template))
    {
        for (int i = 0; i < 24; i++)
        {
            char ph[64];
            snprintf(ph, 64, "<!--%s-->", placeholders[i]);
            replace_placeholder(line, ph, inputs[i]);
        }
        fputs(line, output);
    }

    fclose(template);
    fclose(output);

    printf("index.html generated successfully\n");
    return 0;
}
