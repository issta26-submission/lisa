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
//<ID> 2069
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "([A-Za-z]+)([0-9]+)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    char text_buf[] = "Alice12345";
    int text_len = (int)(sizeof(text_buf) - 1);
    const char rewrite_template[] = "$1-REPL";
    cre2_options_t *opt = NULL;
    cre2_set *set = NULL;
    cre2_regexp_t *re = NULL;
    cre2_string_t matches[3];
    cre2_string_t text_and_target = { text_buf, text_len };
    cre2_string_t rewrite_str = { rewrite_template, (int)(sizeof(rewrite_template) - 1) };
    int one_line_before = 0;
    int one_line_after = 0;
    long checksum = 0;

    // step 2: Setup (initialize options and toggle one_line flag)
    opt = cre2_opt_new();
    one_line_before = cre2_opt_one_line(opt);
    cre2_opt_set_one_line(opt, 1);
    one_line_after = cre2_opt_one_line(opt);

    // step 3: Configure (build a set and add a simple pattern)
    set = cre2_set_new(opt, CRE2_UNANCHORED);
    cre2_set_add_simple(set, pattern);
    cre2_set_compile(set);

    // step 4: Operate (compile regexp, perform match, then perform replace using the regexp)
    re = cre2_new(pattern, pattern_len, opt);
    memset(matches, 0, sizeof(matches));
    int match_count = cre2_match(re, text_buf, text_len, 0, text_len, CRE2_UNANCHORED, matches, 3);
    int replace_result = cre2_replace_re(re, &text_and_target, &rewrite_str);

    // step 5: Validate (produce a checksum combining results so values flow through)
    checksum = (long)pattern_len + (long)match_count + (long)replace_result + (long)one_line_before + (long)one_line_after + (long)matches[0].length + (long)text_and_target.length + (long)(re != NULL) + (long)(set != NULL);
    (void)checksum;

    // step 6: Cleanup (delete regexp, set and options)
    cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}