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
//<ID> 242
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // Create options for compilation
    cre2_options_t *opt = cre2_opt_new();
    if (!opt) return 66;
    cre2_opt_set_case_sensitive(opt, 1);
    cre2_opt_set_perl_classes(opt, 1);

    // Compile a simple pattern into a cre2_regexp_t
    const char *pattern = "a(b)c";
    int pat_len = (int)strlen(pattern);
    cre2_regexp_t *re = cre2_new(pattern, pat_len, opt);
    if (!re) {
        cre2_opt_delete(opt);
        return 66;
    }

    // Query possible match range for the compiled regexp
    cre2_string_t min_range = { nullptr, 0 };
    cre2_string_t max_range = { nullptr, 0 };
    // maxlen is a heuristic max length for the returned strings
    int pmr_rc = cre2_possible_match_range(re, &min_range, &max_range, 16);
    (void)pmr_rc; // ignore result for this test sequence

    // Prepare a text buffer to run find_and_consume on
    char text_buf[] = "zzabczz";
    cre2_string_t text = { text_buf, (int)strlen(text_buf) };

    // Output/storage for the consumed match or rewrite
    char out_buf[128] = {0};
    cre2_string_t out = { out_buf, 0 };

    // Use the pattern-based find_and_consume (pattern supplied as const char *)
    // Note: the integer argument semantics vary by API; pass 1 as a small nmatch placeholder.
    int fac_rc1 = cre2_find_and_consume("b", &text, &out, 1);
    (void)fac_rc1;

    // Reset text and out for regex-based consumption and call cre2_find_and_consume_re
    text.data = text_buf;
    text.length = (int)strlen(text_buf);
    out.data = out_buf;
    out.length = 0;
    int fac_rc2 = cre2_find_and_consume_re(re, &text, &out, 1);
    (void)fac_rc2;

    // Create a pattern set, add a simple pattern, and compile it
    cre2_set *set = cre2_set_new(opt, (cre2_anchor_t)0);
    if (set) {
        cre2_set_add_simple(set, "abc");
        int set_rc = cre2_set_compile(set);
        (void)set_rc;
        cre2_set_delete(set);
    }

    // Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    return 66;
}