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
//<ID> 229
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // Create options and limit memory for compilation
    cre2_options_t *opt = cre2_opt_new();
    if (!opt) return 1;
    cre2_opt_set_max_mem(opt, (int64_t)(1 << 20)); // 1 MiB

    // Compile a regexp with two capturing groups to be used with rewrite checks
    const char *pattern = "(a)(b)";
    int pattern_len = 6; // "(a)(b)"
    cre2_regexp_t *rex = cre2_new(pattern, pattern_len, opt);
    if (!rex) {
        cre2_opt_delete(opt);
        return 2;
    }

    // Prepare a rewrite string and an errmsg buffer for cre2_check_rewrite_string
    char rewrite_buf[32] = "$1-$2";
    cre2_string_t rewrite = { rewrite_buf, 5 }; // "$1-$2" length 5

    char errmsg_buf[256];
    // Initialize errmsg buffer content to zeros and set length to capacity
    memset(errmsg_buf, 0, sizeof(errmsg_buf));
    cre2_string_t errmsg = { errmsg_buf, (int)sizeof(errmsg_buf) };

    // Check the rewrite string validity against the compiled regexp
    int check_res = cre2_check_rewrite_string(rex, &rewrite, &errmsg);
    // We won't treat a non-zero/zero specifically as fatal here; continue to exercise other APIs.

    // Demonstrate cre2_quote_meta: quote a meta-containing original string into quoted_buf
    const char *original_literal = "a+b(c)*";
    cre2_string_t original = { original_literal, 7 }; // "a+b(c)*" length 7
    char quoted_buf[128];
    memset(quoted_buf, 0, sizeof(quoted_buf));
    cre2_string_t quoted = { quoted_buf, (int)sizeof(quoted_buf) };
    int quote_res = cre2_quote_meta(&quoted, &original);
    // quote_res is not required to be checked here for success; continue.

    // Use cre2_easy_match (no compiled regexp) to match a simple pattern against text
    const char *simple_pat = "ab";
    int simple_pat_len = 2;
    char text_buf[] = "ab";
    int text_len = 2;

    // Prepare match array (nmatch = 3) to receive match parts
    cre2_string_t matches[3];
    for (int i = 0; i < 3; ++i) {
        matches[i].data = NULL;
        matches[i].length = 0;
    }

    int match_res = cre2_easy_match(simple_pat, simple_pat_len, text_buf, text_len, matches, 3);
    if (!match_res) {
        // cleanup and fail if simple match didn't succeed
        cre2_delete(rex);
        cre2_opt_delete(opt);
        return 3;
    }

    // Clean up created objects
    cre2_delete(rex);
    cre2_opt_delete(opt);

    // If we reached here, we've successfully exercised the requested API sequence
    return 66;
}