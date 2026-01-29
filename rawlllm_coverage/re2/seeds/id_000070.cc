#include <cre2.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <cstring>
#include <fcntl.h>
//<ID> 70
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // Create options and query current log_errors flag.
    cre2_options_t *opt = cre2_opt_new();
    if (opt == nullptr) {
        return 1; // failed to allocate options
    }
    int log_errors_flag = cre2_opt_log_errors(opt);

    // Prepare an original string to be quoted by cre2_quote_meta.
    cre2_string_t original;
    original.data = "a+b"; // characters with regex metacharacters
    original.length = 3;

    // Prepare an (initially empty) quoted output struct and call cre2_quote_meta.
    cre2_string_t quoted;
    quoted.data = nullptr;
    quoted.length = 0;
    int quote_res = cre2_quote_meta(&quoted, &original);
    (void)quote_res; // accept result for fuzzing; not required to handle here
    (void)quoted;    // quoted may be populated by the library

    // Prepare text and rewrite strings for cre2_replace.
    const char *pattern = "foo";
    cre2_string_t text_and_target;
    text_and_target.data = "foo bar foo";
    text_and_target.length = 11; // length of "foo bar foo"

    cre2_string_t rewrite;
    rewrite.data = "baz";
    rewrite.length = 3;

    // Perform a single replace operation using the simple pattern.
    int replace_res = cre2_replace(pattern, &text_and_target, &rewrite);
    (void)replace_res; // accept result; primary goal is exercising the API

    // Clean up options.
    cre2_opt_delete(opt);

    return 66;
}