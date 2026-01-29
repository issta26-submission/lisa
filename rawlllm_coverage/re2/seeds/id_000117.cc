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
//<ID> 117
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // Create options object
    cre2_options_t *opt = cre2_opt_new();
    if (!opt) return 1;

    // Configure options: set one-line mode and encoding
    cre2_opt_set_one_line(opt, 1);
    int one_line_flag = cre2_opt_one_line(opt); // required API usage

    cre2_opt_set_encoding(opt, CRE2_UTF8);
    cre2_encoding_t enc = cre2_opt_encoding(opt); // required API usage

    // Prepare a pattern and compile it with the options
    const char pattern[] = "(foo)(bar)";
    cre2_regexp_t *rex = cre2_new(pattern, (int)(sizeof(pattern) - 1), opt); // required API usage
    if (!rex) {
        cre2_opt_delete(opt);
        return 2;
    }

    // Prepare text and rewrite strings for global replace
    const char text_buf[] = "before foobar after foobar";
    cre2_string_t text_and_target = { text_buf, (int)(sizeof(text_buf) - 1) };

    const char rewrite_buf[] = "REPL";
    cre2_string_t rewrite = { rewrite_buf, (int)(sizeof(rewrite_buf) - 1) };

    // Perform a global replace using the compiled regex
    int replace_result = cre2_global_replace_re(rex, &text_and_target, &rewrite); // required API usage

    // Use cre2_easy_match to do a simple match (pattern given directly)
    cre2_string_t matches[3]; // one for whole match + two capture groups
    int easy_match_result = cre2_easy_match(pattern, (int)(sizeof(pattern) - 1),
                                            text_buf, (int)(sizeof(text_buf) - 1),
                                            matches, 3); // required API usage

    // Clean up
    cre2_delete(rex);
    cre2_opt_delete(opt);

    // Use (void) casts to avoid unused-variable warnings, values could be inspected in a real test harness
    (void)one_line_flag;
    (void)enc;
    (void)replace_result;
    (void)easy_match_result;

    return 66;
}