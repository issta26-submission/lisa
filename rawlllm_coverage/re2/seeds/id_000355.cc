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
//<ID> 355
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // Create options
    cre2_options_t *opt = cre2_opt_new();
    if (!opt) return 1;

    // Prepare a pattern as an array so we can use sizeof() for length without needing strlen()
    const char pattern[] = "(a(b)c)";
    int pattern_len = (int)(sizeof(pattern) - 1);

    // Compile a regexp using cre2_new
    cre2_regexp_t *re = cre2_new(pattern, pattern_len, opt);
    if (!re) {
        cre2_opt_delete(opt);
        return 2;
    }

    // Query number of capturing groups
    int num_groups = cre2_num_capturing_groups(re);
    (void)num_groups; // silence unused-variable warnings in this test harness

    // Create a set and compile it (demonstrates cre2_set_compile use)
    cre2_set *set = cre2_set_new(opt, (cre2_anchor_t)0);
    if (set) {
        // add a simple pattern to the set (best-effort; ignore return)
        cre2_set_add_simple(set, "test");
        // Compile the set
        int set_rc = cre2_set_compile(set);
        (void)set_rc;
        // Clean up the set
        cre2_set_delete(set);
    }

    // Demonstrate cre2_quote_meta: quote a string that contains regex metacharacters
    const char original_text[] = "a+b(c)*";
    cre2_string_t original = { original_text, (int)(sizeof(original_text) - 1) };
    char quoted_buf[128];
    cre2_string_t quoted = { quoted_buf, (int)sizeof(quoted_buf) };
    int quote_rc = cre2_quote_meta(&quoted, &original);
    (void)quote_rc; // ignore exact return for this test

    // Demonstrate cre2_easy_match: attempt a match and collect up to 3 matches/groups
    const char haystack[] = "xxabcxx";
    int haystack_len = (int)(sizeof(haystack) - 1);
    cre2_string_t matches[3];
    char match_buf0[64], match_buf1[64], match_buf2[64];
    matches[0].data = match_buf0; matches[0].length = (int)sizeof(match_buf0);
    matches[1].data = match_buf1; matches[1].length = (int)sizeof(match_buf1);
    matches[2].data = match_buf2; matches[2].length = (int)sizeof(match_buf2);

    int em_rc = cre2_easy_match(pattern, pattern_len, haystack, haystack_len, matches, 3);
    (void)em_rc;

    // Clean up regex and options
    cre2_delete(re);
    cre2_opt_delete(opt);

    // If we reached here, treat as success for this API sequence test
    return 66;
}