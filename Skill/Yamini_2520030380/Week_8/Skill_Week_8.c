#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT 1024
#define MAX_OUTPUT 2048

void trim_newline(char *str)
{
    str[strcspn(str, "\n")] = '\0';
}

const char *get_variable_value(const char *name)
{
    const char *value = getenv(name);

    if (value != NULL)
        return value;

    return NULL;
}

void expand_variables(const char *input, char *output, size_t output_size)
{
    size_t i = 0;
    size_t j = 0;

    while (input[i] != '\0' && j < output_size - 1)
    {
        if (input[i] == '$')
        {
            char variable[128];
            size_t k = 0;

            i++;

            if (input[i] == '{')
            {
                i++;

                while (input[i] != '\0' &&
                       input[i] != '}' &&
                       k < sizeof(variable) - 1)
                {
                    variable[k++] = input[i++];
                }

                if (input[i] == '}')
                    i++;
            }
            else
            {
                while (input[i] != '\0' &&
                       (isalnum((unsigned char)input[i]) ||
                        input[i] == '_') &&
                       k < sizeof(variable) - 1)
                {
                    variable[k++] = input[i++];
                }
            }

            variable[k] = '\0';

            if (k == 0)
            {
                output[j++] = '$';
                continue;
            }

            const char *value = get_variable_value(variable);

            if (value != NULL)
            {
                size_t value_len = strlen(value);

                if (j + value_len < output_size)
                {
                    strcpy(&output[j], value);
                    j += value_len;
                }
            }
            else
            {
                printf("Undefined variable: %s\n", variable);
            }
        }
        else
        {
            output[j++] = input[i++];
        }
    }

    output[j] = '\0';
}

void demonstrate_expansion(const char *input)
{
    char output[MAX_OUTPUT];

    printf("\nInput    : %s\n", input);

    expand_variables(input, output, sizeof(output));

    printf("Expanded : %s\n", output);
}

int main()
{
    printf("===============================================\n");
    printf("       SKILL WEEK 8 - VARIABLE EXPANSION\n");
    printf("===============================================\n");

    setenv("USER_NAME", "Yamini", 1);
    setenv("PROJECT", "OS-Vanguard", 1);
    setenv("WEEK", "8", 1);

    printf("\nEnvironment variables configured:\n");
    printf("USER_NAME = %s\n", getenv("USER_NAME"));
    printf("PROJECT   = %s\n", getenv("PROJECT"));
    printf("WEEK      = %s\n", getenv("WEEK"));

    printf("\n---------- VARIABLE EXPANSION ----------\n");

    demonstrate_expansion("Hello $USER_NAME");
    demonstrate_expansion("Project: $PROJECT");
    demonstrate_expansion("Current week: ${WEEK}");

    printf("\n---------- MULTIPLE VARIABLES ----------\n");

    demonstrate_expansion(
        "$USER_NAME is working on $PROJECT - Week $WEEK");

    printf("\n---------- UNDEFINED VARIABLE ----------\n");

    demonstrate_expansion(
        "Value: $UNDEFINED_VARIABLE");

    printf("\n---------- NESTED EXPRESSION ----------\n");

    demonstrate_expansion(
        "$PROJECT/$USER_NAME/week_$WEEK");

    printf("\n===============================================\n");
    printf("       SKILL WEEK 8 COMPLETED SUCCESSFULLY\n");
    printf("===============================================\n");

    return 0;
}
