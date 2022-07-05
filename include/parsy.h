#ifndef PARSY_H
#define PARSY_H

#include <stddef.h>
#include <stdbool.h>

/**
 * \struct parsy argument entry
 * 
 * \brief This sturcture represent an argument entry.
 * 
 * \var opt: The option name (such as -h).
 * \var optlong: The option longname (such as --help).
 * \var help: The help message.
 * \var type_info: A string that represent the type of the argument.
 * \var argument_needed: A boolean that represent if an argument is needed after the option.
 */
struct parsy_entry {
    const char *opt;
    const char *optlong;
    const char *help;
    const char *type_info;
    int (*handler)(void *userdata, int argc, char *args[]);
    bool argument_needed;
};

/**
 * \struct parsy_ctx, a container for a parsy context
 * 
 * \brief This structure represent a parsy context.
 * 
 * \var program_name: The name of the program.
 * \var entries: The entries of the context.
 * \var entries_size: The size of the entries.
 * \var userdata: The userdata of the context, passed to each function pointer.
 */
struct parsy_ctx {
    const char *program_name;
    const struct parsy_entry *entries;
    size_t entries_size;
    void *userdata;
};

#define PARSY_DEFAULT_HELP_HANDLER ((int (*)(void *userdata, int argc, char *args[]))-1)

int parsy_parse(int argc, char *argv[], const struct parsy_ctx *ctx) __nonnull((3));

#endif /* !PARSY_H */