#include <stdio.h>
#include <string.h>

#include "parsy.h"
#define ERR_LOG(...) fprintf(stderr, __VA_ARGS__)
#define LOG(...) fprintf(stdout, __VA_ARGS__)

static void parsy_display_help(const struct parsy_ctx *ctx)
{
    LOG("USAGE: %s [OPTIONS]\n", ctx->program_name);
    for (size_t i = 0; i < ctx->entries_size; ++i) {
        LOG("\n  %s", ctx->entries[i].opt);
        if (ctx->entries[i].optlong != NULL) {
            LOG(", %s", ctx->entries[i].optlong);
        }
        if (ctx->entries[i].type_info != NULL) {
            LOG(" [%s]\n", ctx->entries[i].type_info);
        } else {
            LOG("\n");
        }
        LOG("\t%s\n", ctx->entries[i].help);
    }
}

static bool is_opt_valid(const struct parsy_entry *entry, const char *arg)
{
    return strcmp(arg, entry->opt) == 0 || strcmp(arg, entry->optlong) == 0;
}

static int handle_argument_entry(int argc, char *argv[], const struct parsy_entry *entry, const struct parsy_ctx *ctx)
{
    int ret = 0;

    if (entry->argument_needed && argc == 1) {
        ERR_LOG("Option: '%s' needs an argument.\n", argv[0]);
        return 1;
    } else if (entry->handler == (typeof(entry->handler))PARSY_DEFAULT_HELP_HANDLER) {
        parsy_display_help(ctx);
        return 0;
    } else if (entry->handler != NULL) {
        ret = entry->handler(ctx->userdata, argc, argv);
        if (ret != 0)
            ERR_LOG("Invalid parameter after '%s' option.\n", argv[0]);
        return ret;
    }
    return 0;
}

static int check_opt(int argc, char *argv[], const struct parsy_ctx *ctx)
{
    for (size_t i = 0; i < ctx->entries_size; ++i) {
        if (is_opt_valid(&ctx->entries[i], argv[0])) {
            return handle_argument_entry(argc, argv, &ctx->entries[i], ctx);
        }
    }
    ERR_LOG("Unkown parameter: '%s'.\n", argv[0]);
    return 1;
}

int parsy_parse(int argc, char *argv[], const struct parsy_ctx *ctx)
{
    int ret = 0;
    int check;

    for (int i = 0; i < argc; ++i) {
        if (argv[i][0] == '-') {
            check = check_opt(argc - i, &argv[i], ctx);
            if (check < 0) {
                return check;
            }
            ret += check;
        }
    }
    return ret;
}