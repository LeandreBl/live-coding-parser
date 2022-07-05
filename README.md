# parsy
A lightweight C main arguments parser

This library is an automated way of generating an USAGE help and handling arguments of the main.

Here is a code example on how to use it:

```c
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <parsy.h>

struct fake_params {
    char *name;
    int age;
};

/**
 * This function will be called each time a "-n" or "--name" is found, see below
 * av[0] holds the opt (-n or --name)
 * av[1] holds the parameter of the opt
 */
static int parser_name_handler(void *userdata, int ac, char *av[])
{
    // Cast the userdata as we know it's type (be careful)
    struct fake_params *params = userdata;
    char *tmp = strdup(av[1]);

    if (tmp == NULL)
        return -1;
    params->name = tmp;
    return 0;
}

/**
 * This function will be called each time a "-a" or "--age" is found, see below
 * av[0] holds the opt (-a or --age)
 * av[1] holds the parameter of the opt
 */
static int parser_age_handler(void *userdata, int ac, char *av[])
{
    // Cast the userdata as we know it's type (be careful)
    struct fake_params *params = userdata;
    int age;
    int ret = sscanf(av[1], "%d", &age);

    if (ret == 1) {
        params->age = age;
        return 0;
    }
    return 1;
}

/**
 * This array is the representation of the options we want to handle
 */
static const struct parsy_entry MAIN_PARSER_ENTRIES[] = {
    {
        .opt = "-h",
        .optlong = "--help",
        .help = "Display this help.",
        .argument_needed = false,
        .type_info = NULL,
        // We'll use the automaticly generated parsy USAGE
        .handler = PARSY_DEFAULT_HELP_HANDLER,
    },
    {
        .opt = "-n",
        .optlong = "--name",
        .help = "Set the name.",
        .argument_needed = true,
        .type_info = "STRING",
        .handler = &parser_name_handler,
    },
    {
        .opt = "-a",
        .optlong = "--age",
        .help = "Set the age.",
        .argument_needed = true,
        .type_info = "INTEGER",
        .handler = &parser_age_handler,
    }
};

int main(int ac, char *av[])
{
    int ret;
    // This will be our structure that will contains our parsed elements
    struct fake_params params = {
        .age = -1,
        .name = NULL
    };
    const struct parsy_ctx ctx = {
        .entries = MAIN_PARSER_ENTRIES,
        .entries_size = sizeof(MAIN_PARSER_ENTRIES) / sizeof(*MAIN_PARSER_ENTRIES),
        .program_name = av[0],
        .userdata = &params,
    };
    // Ask parsy to parse the arguments, we will omit the first argument (which is the program name)
    ret = parsy_parse(ac - 1, av + 1, &ctx);
    printf("Parser finished with return code: %d\n", ret);
    printf("{name: '%s', age: %d}\n", params.name, params.age);
    free(params.name);
    return 0;
}
```
`./example -a 80`
```
Parser finished with return code: 0
{name: '(null)', age: 80}
```
`./example -a 80 --name "Foo"`
```
Parser finished with return code: 0
{name: 'foo', age: 80}
```
`./example -h`
```
USAGE: ./a.out [OPTIONS]

  -h, --help
        Display this help.

  -n, --name [STRING]
        Set the name.

  -a, --age [INTEGER]
        Set the age.
Parser finished with return code: 1
{name: '(null)', age: -1}
```