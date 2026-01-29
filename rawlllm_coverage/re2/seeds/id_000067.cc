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
//<ID> 67
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // local helper to compute C-string length without relying on <cstring>
    auto cstrlen = [](const char *s) -> int {
        if (!s) return 0;
        int n = 0;
        while (s[n]) ++n;
        return n;
    };

    // create options
    cre2_options_t *opt = cre2_opt_new();
    if (!opt) {
        return 1;
    }

    // query the encoding option (use of cre2_opt_encoding)
    cre2_encoding_t enc = cre2_opt_encoding(opt);
    (void)enc; // just exercise the API; value not critical for this test

    // create a set with the given options (use of cre2_set_new)
    cre2_set *set = cre2_set_new(opt, CRE2_UNANCHORED);
    if (!set) {
        cre2_opt_delete(opt);
        return 2;
    }

    // compile a regex from a pattern (use of cre2_new)
    const char *pattern = "num(\\d+)";
    int pattern_len = cstrlen(pattern);
    cre2_regexp_t *re = cre2_new(pattern, pattern_len, opt);
    if (!re) {
        cre2_set_delete(set);
        cre2_opt_delete(opt);
        return 3;
    }

    // prepare text and match structures for find_and_consume (use of cre2_find_and_consume)
    cre2_string_t text;
    const char *text_c = "num12345_tail";
    text.data = text_c;
    text.length = cstrlen(text_c);

    cre2_string_t match;
    match.data = "";
    match.length = 0;

    // attempt to find and consume using the pattern string API
    int found = cre2_find_and_consume(pattern, &text, &match, 1);
    (void)found; // we don't enforce a particular result for this test

    // prepare for a replace call (use of cre2_replace)
    cre2_string_t text_and_target;
    const char *replace_input = "replace num123 and num456";
    text_and_target.data = replace_input;
    text_and_target.length = cstrlen(replace_input);

    cre2_string_t rewrite;
    const char *rewrite_s = "N";
    rewrite.data = rewrite_s;
    rewrite.length = cstrlen(rewrite_s);

    int replace_result = cre2_replace(pattern, &text_and_target, &rewrite);
    (void)replace_result; // accept any result as part of exercising the API

    // cleanup
    cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    return 66;
}