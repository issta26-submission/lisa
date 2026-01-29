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
//<ID> 247
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // Create options and query the 'literal' flag
    cre2_options_t *opt = cre2_opt_new();
    if (!opt) return 1;
    int literal_flag = cre2_opt_literal(opt);
    (void)literal_flag; // silence unused warning

    // Prepare a pattern and compile it with the options
    const char *pattern = "(foo)(bar)?";
    int pattern_len = (int)(sizeof("(foo)(bar)?") - 1);
    cre2_regexp_t *re = cre2_new(pattern, pattern_len, opt);
    if (!re) {
        cre2_opt_delete(opt);
        return 2;
    }

    // Prepare input text for consume_re
    const char *input = "foobar baz foo";
    cre2_string_t text = { input, (int)(sizeof("foobar baz foo") - 1) };

    // Prepare an array for matches (full match + two capturing groups)
    const int NMATCH = 3;
    cre2_string_t matches[NMATCH];
    for (int i = 0; i < NMATCH; ++i) {
        matches[i].data = NULL;
        matches[i].length = 0;
    }

    // Attempt to consume from the start of the text using the compiled regex
    int consume_ret = cre2_consume_re(re, &text, matches, NMATCH);
    (void)consume_ret; // we record the result but continue for further API usage

    // Prepare a rewrite string and a copy of the text for global replace
    cre2_string_t rewrite = { "X", (int)(sizeof("X") - 1) };
    cre2_string_t text_and_target = text; // copy the original text structure

    // Perform a global replace using the compiled regex
    int replace_count = cre2_global_replace_re(re, &text_and_target, &rewrite);
    (void)replace_count;

    // Clean up
    cre2_delete(re);
    cre2_opt_delete(opt);

    return 66;
}