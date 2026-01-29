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
//<ID> 161
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // Create options and set never_nl, then read it back via cre2_opt_never_nl
    cre2_options_t *opt = cre2_opt_new();
    if (!opt) return 1;
    cre2_opt_set_never_nl(opt, 1);
    int never_nl_flag = cre2_opt_never_nl(opt);
    (void)never_nl_flag; // silence unused-variable warnings if any

    // Create a set and add a simple pattern to it
    cre2_set *set = cre2_set_new(opt, (cre2_anchor_t)0);
    if (!set) {
        cre2_opt_delete(opt);
        return 2;
    }
    int add_res = cre2_set_add_simple(set, "foo");
    (void)add_res;

    // Create a compiled regexp from a pattern
    const char *pattern = "(foo)";
    int pattern_len = (int)strlen(pattern);
    cre2_regexp_t *rex = cre2_new(pattern, pattern_len, opt);

    // Prepare a rewrite string and an errmsg buffer and check the rewrite string
    char errmsg_buf[256] = {0};
    cre2_string_t rewrite = { "bar", 3 };
    cre2_string_t errmsg = { errmsg_buf, 0 };
    if (rex) {
        int check = cre2_check_rewrite_string(rex, &rewrite, &errmsg);
        (void)check;
    }

    // Prepare text and match structures and attempt to find and consume with the compiled regex
    char text_buf[] = "foo123";
    cre2_string_t text = { text_buf, (int)strlen(text_buf) };
    cre2_string_t match = { NULL, 0 }; // single-slot for captures
    if (rex) {
        // nmatch = 1 (one capture slot)
        int found = cre2_find_and_consume_re(rex, &text, &match, 1);
        (void)found;
    }

    // Use cre2_replace with a simple pattern on a mutable target buffer
    char target_buf[128] = "hello world";
    cre2_string_t text_and_target = { target_buf, (int)strlen(target_buf) };
    cre2_string_t rewrite2 = { "O", 1 };
    int replace_ret = cre2_replace("l", &text_and_target, &rewrite2);
    (void)replace_ret;

    // Cleanup
    if (rex) cre2_delete(rex);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    return 66;
}