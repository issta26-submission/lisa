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
//<ID> 2066
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "([A-Za-z]+)-([0-9]+)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    char text_and_target[64] = "Alice-12345";
    const int initial_text_len = (int)(sizeof("Alice-12345") - 1);
    const char rewrite[] = "$1-REPL";
    cre2_options_t *opt = NULL;
    cre2_set *set = NULL;
    cre2_regexp_t *re = NULL;
    cre2_string_t text_str;
    cre2_string_t rewrite_str;
    cre2_string_t matches[3];
    int one_line_before = 0;
    int one_line_after = 0;
    int match_result = 0;
    int replace_result = 0;
    long checksum = 0;

    // step 2: Setup (options)
    opt = cre2_opt_new();
    one_line_before = cre2_opt_one_line(opt);
    cre2_opt_set_one_line(opt, 1);
    one_line_after = cre2_opt_one_line(opt);

    // step 3: Configure (set and regexp)
    set = cre2_set_new(opt, CRE2_UNANCHORED);
    cre2_set_add_simple(set, pattern);
    cre2_set_compile(set);
    re = cre2_new(pattern, pattern_len, opt);

    // step 4: Operate (matching and replacement)
    memset(matches, 0, sizeof(matches));
    text_str.data = text_and_target;
    text_str.length = initial_text_len;
    rewrite_str.data = rewrite;
    rewrite_str.length = (int)(sizeof(rewrite) - 1);
    match_result = cre2_match(re, text_and_target, text_str.length, 0, text_str.length, CRE2_UNANCHORED, matches, 3);
    replace_result = cre2_replace_re(re, &text_str, &rewrite_str);

    // step 5: Validate (aggregate results into checksum)
    checksum = (long)pattern_len + (long)one_line_before + (long)one_line_after + (long)match_result + (long)replace_result + (long)(re != NULL) + (long)(set != NULL) + (long)text_str.length;
    (void)checksum;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}