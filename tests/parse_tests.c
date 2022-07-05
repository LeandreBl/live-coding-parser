#include <criterion/criterion.h>
#include <stdio.h>
#include "parsy.h"

struct test {
    int i;
    float f;
    char *s;
};

static int i_handler(void *userdata, int ac, char *av[])
{
    struct test *t = userdata;
    int tmp;
    int ret = sscanf(av[1], "%i", &tmp);

    (void)ac;
    if (ret == 1) {
        t->i = tmp;
        return 0;
    }
    return 1;
}

static int f_handler(void *userdata, int ac, char *av[])
{
    struct test *t = userdata;
    float tmp;
    int ret = sscanf(av[1], "%f", &tmp);

    (void)ac;
    if (ret == 1) {
        t->f = tmp;
        return 0;
    }
    return 1;
}

static int s_handler(void *userdata, int ac, char *av[])
{
    struct test *t = userdata;
    char *tmp = strdup(av[1]);

    (void)ac;
    if (tmp == NULL) {
        return -1;
    }
    t->s = tmp;
    return 0;
}

Test(parsy_empty, empty_argv)
{
    struct test t = {
        .i = 0,
        .f = 0,
        .s = NULL,
    };
    const struct parsy_ctx ctx = {
        .entries = NULL,
        .entries_size = 0,
        .program_name = "test",
        .userdata = &t
    };
    int ret = parsy_parse(0, (char *[]){NULL}, &ctx);
    cr_assert(ret == 0);
    cr_assert(t.i == 0);
    cr_assert(t.f == 0);
    cr_assert(t.s == NULL);
}

Test(parsy_empty, one_integer_arg)
{
    const struct parsy_entry PARSER_OPTIONS[] = {
        {
            .opt = "-i",
            .optlong = "--int",
            .type_info = "INTEGER",
            .help = "",
            .handler = &i_handler,
        }
    };
    struct test t = {
        .i = 0,
        .f = 0,
        .s = NULL,
    };
    const struct parsy_ctx ctx = {
        .entries = PARSER_OPTIONS,
        .entries_size = sizeof(PARSER_OPTIONS) / sizeof(*PARSER_OPTIONS),
        .program_name = "test",
        .userdata = &t
    };
    int ret = parsy_parse(2, (char *[]){"-i", "83"}, &ctx);
    cr_assert(ret == 0);
    cr_assert(t.i == 83);
    cr_assert(t.f == 0);
    cr_assert(t.s == NULL);
}

Test(parsy_empty, one_float_arg)
{
    const struct parsy_entry PARSER_OPTIONS[] = {
        {
            .opt = "-f",
            .optlong = "--float",
            .type_info = "FLOAT",
            .help = "",
            .handler = &f_handler,
        }
    };
    struct test t = {
        .i = 0,
        .f = 0,
        .s = NULL,
    };
    const struct parsy_ctx ctx = {
        .entries = PARSER_OPTIONS,
        .entries_size = sizeof(PARSER_OPTIONS) / sizeof(*PARSER_OPTIONS),
        .program_name = "test",
        .userdata = &t
    };
    int ret = parsy_parse(2, (char *[]){"-f", "3.14"}, &ctx);
    cr_assert(ret == 0);
    cr_assert(t.i == 0);
    cr_assert(t.f == 3.14f);
    cr_assert(t.s == NULL);
}