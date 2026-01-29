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
//<ID> 43
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // Create options for regex/set creation
    cre2_options_t *opt = cre2_opt_new();
    if (!opt) return 1;

    // Create a set, add a simple pattern, and compile it.
    cre2_set *set = cre2_set_new(opt, (cre2_anchor_t)0);
    if (!set) {
        cre2_opt_delete(opt);
        return 1;
    }
    // Add a simple pattern to the set
    cre2_set_add_simple(set, "foo|bar");
    // Compile the set (check result but proceed to cleanup if it fails)
    int set_rc = cre2_set_compile(set);
    (void)set_rc; // we don't strictly require a particular return value here

    // Prepare a regex via cre2_new
    const char pat[] = "(foo)bar";
    cre2_regexp_t *rex = cre2_new(pat, (int)(sizeof(pat) - 1), opt);
    if (!rex) {
        cre2_set_delete(set);
        cre2_opt_delete(opt);
        return 1;
    }

    // Prepare mutable buffers for text and rewrite arguments
    char text_buf1[] = "foobarbaz";
    cre2_string_t text1 = { text_buf1, (int)(sizeof(text_buf1) - 1) };
    char rewrite_buf1[] = "X";
    cre2_string_t rewrite1 = { rewrite_buf1, (int)(sizeof(rewrite_buf1) - 1) };

    // Use cre2_replace_re with the compiled regexp
    int rc_replace_re = cre2_replace_re(rex, &text1, &rewrite1);
    (void)rc_replace_re;

    // Use cre2_replace (pattern-based) on another buffer
    char text_buf2[] = "oohello";
    cre2_string_t text2 = { text_buf2, (int)(sizeof(text_buf2) - 1) };
    char rewrite_buf2[] = "Y";
    cre2_string_t rewrite2 = { rewrite_buf2, (int)(sizeof(rewrite_buf2) - 1) };

    int rc_replace = cre2_replace("o+", &text2, &rewrite2);
    (void)rc_replace;

    // Clean up
    cre2_delete(rex);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    return 66;
}