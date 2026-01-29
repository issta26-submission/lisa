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
//<ID> 297
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // Create and configure options
    cre2_options_t *opt = cre2_opt_new();
    if (opt) {
        // set literal mode for the options (use setter available in API list)
        cre2_opt_set_literal(opt, 1);
        // read back the literal flag using the required API
        int literal_flag = cre2_opt_literal(opt);
        (void)literal_flag; // silence unused-variable warning
    }

    // Compile a simple literal regexp "abc" using the options
    const char pattern[] = "abc";
    cre2_regexp_t *re = cre2_new(pattern, (int)sizeof(pattern) - 1, opt);

    // Prepare a subject string for consume operation: "abcxyz"
    cre2_string_t subject = { "abcxyz", 6 };
    cre2_string_t match = { nullptr, 0 };

    // Attempt to consume a match from the start of subject using the compiled regexp
    // nmatch = 1 (only the whole match)
    int consume_res = 0;
    if (re) {
        consume_res = cre2_consume_re(re, &subject, &match, 1);
    }
    (void)consume_res;

    // Query possible match range for the regex (min and max)
    cre2_string_t min_s = { nullptr, 0 };
    cre2_string_t max_s = { nullptr, 0 };
    int possible_range_res = 0;
    if (re) {
        possible_range_res = cre2_possible_match_range(re, &min_s, &max_s, 10);
    }
    (void)possible_range_res;

    // Prepare data for a full match check: use exact "abc" which should fully match the regex
    cre2_string_t full_text = { "abc", 3 };
    cre2_string_t out_matches[2];
    out_matches[0].data = nullptr; out_matches[0].length = 0;
    out_matches[1].data = nullptr; out_matches[1].length = 0;

    int full_match_res = 0;
    if (re) {
        full_match_res = cre2_full_match_re(re, &full_text, out_matches, 1);
    }
    (void)full_match_res;

    // Clean up
    if (re) {
        cre2_delete(re);
    }
    if (opt) {
        cre2_opt_delete(opt);
    }

    return 66;
}