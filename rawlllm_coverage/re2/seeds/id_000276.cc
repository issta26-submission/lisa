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
//<ID> 276
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // Create options and set "never capture"
    cre2_options_t *opt = cre2_opt_new();
    if (!opt) return 1;
    cre2_opt_set_never_capture(opt, 1);

    // Create a set with the options (anchor value cast to cre2_anchor_t)
    cre2_set *set = cre2_set_new(opt, (cre2_anchor_t)0);
    if (!set) {
        cre2_opt_delete(opt);
        return 2;
    }

    // Add a pattern to the set
    const char *pattern = "foo(.*)bar";
    size_t pattern_len = strlen(pattern);
    char add_errbuf[256] = {0};
    // Provide an error buffer; ignore the detailed return here but keep the call
    (void)cre2_set_add(set, pattern, pattern_len, add_errbuf, sizeof(add_errbuf));

    // Compile the set (best-effort; ignore the compile return for the test harness)
    (void)cre2_set_compile(set);

    // Also create a compiled regexp using the same options to test rewrite checking
    cre2_regexp_t *rex = cre2_new(pattern, (int)pattern_len, opt);
    if (!rex) {
        cre2_set_delete(set);
        cre2_opt_delete(opt);
        return 3;
    }

    // Use cre2_easy_match to perform a quick match against some text
    const char *text = "fooxbar";
    int text_len = (int)strlen(text);
    cre2_string_t match;
    match.data = NULL;
    match.length = 0;
    int easy_match_res = cre2_easy_match(pattern, (int)pattern_len, text, text_len, &match, 1);
    (void)easy_match_res; // result not required for this sequence, but call is important

    // Prepare a rewrite string and an error message container for checking the rewrite
    const char *rewrite_s = "$1";
    cre2_string_t rewrite;
    rewrite.data = rewrite_s;
    rewrite.length = (int)strlen(rewrite_s);

    char errmsg_buf[256] = {0};
    cre2_string_t errmsg;
    errmsg.data = errmsg_buf;
    errmsg.length = 0;

    // Check the rewrite string against the compiled regexp
    int check_res = cre2_check_rewrite_string(rex, &rewrite, &errmsg);
    (void)check_res; // accept any outcome; ensure the API is exercised

    // Cleanup
    cre2_delete(rex);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    return 66;
}