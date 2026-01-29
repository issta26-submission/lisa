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
//<ID> 319
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // Create options and enable logging
    cre2_options_t *opt = cre2_opt_new();
    if (!opt) return 1;

    cre2_opt_set_log_errors(opt, 1);
    int posix_flag = cre2_opt_posix_syntax(opt); // query POSIX flag (may be 0/1)

    // Prepare a pattern with a named capturing group "g"
    const char pattern[] = "(?P<g>ab)c";
    int pattern_len = (int)(sizeof(pattern) - 1);

    // Compile the regexp using the options
    cre2_regexp_t *re = cre2_new(pattern, pattern_len, opt);
    if (!re) {
        cre2_opt_delete(opt);
        return 2;
    }

    // Look up the named capturing group index for "g"
    int group_index = cre2_find_named_capturing_groups(re, "g");

    // Prepare input text and a match holder for find_and_consume
    cre2_string_t text;
    text.data = "abc";
    text.length = 3;

    cre2_string_t match;
    match.data = NULL;
    match.length = 0;

    // Use the pattern-based find-and-consume API (non-_re variant)
    int fa_consume_result = cre2_find_and_consume(pattern, &text, &match, 0);

    // Silence unused-variable warnings by casting to void
    (void)posix_flag;
    (void)group_index;
    (void)fa_consume_result;
    (void)match;

    // Clean up
    cre2_delete(re);
    cre2_opt_delete(opt);

    return 66;
}