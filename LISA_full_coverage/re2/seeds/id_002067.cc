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
//<ID> 2067
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
    const char initial_text[] = "Bob-4242";
    const int initial_text_len = (int)(sizeof(initial_text) - 1);
    const char rewrite_template[] = "$1_ID_$2";
    cre2_options_t *opt = NULL;
    cre2_set *set = NULL;
    cre2_regexp_t *re = NULL;
    int one_line_before = 0;
    int one_line_after = 0;
    char text_buf[128];
    char rewrite_buf[64];
    cre2_string_t text_and_target;
    cre2_string_t rewrite;
    cre2_string_t matches[3];
    int match_rc = 0;
    int replace_rc = 0;
    long checksum = 0;

    // step 2: Setup (options, flags and set)
    opt = cre2_opt_new();
    one_line_before = cre2_opt_one_line(opt);
    cre2_opt_set_one_line(opt, 1);
    one_line_after = cre2_opt_one_line(opt);
    set = cre2_set_new(opt, CRE2_UNANCHORED);
    cre2_set_add_simple(set, pattern);
    cre2_set_compile(set);

    // step 3: Build regexp and prepare buffers
    re = cre2_new(pattern, pattern_len, opt);
    memset(text_buf, 0, sizeof(text_buf));
    memcpy(text_buf, initial_text, (size_t)initial_text_len);
    text_and_target.data = text_buf;
    text_and_target.length = initial_text_len;
    memset(rewrite_buf, 0, sizeof(rewrite_buf));
    memcpy(rewrite_buf, rewrite_template, (size_t)(sizeof(rewrite_template) - 1));
    rewrite.data = rewrite_buf;
    rewrite.length = (int)(sizeof(rewrite_template) - 1);
    memset(matches, 0, sizeof(matches));

    // step 4: Operate (perform a match and a replace using the compiled regexp)
    match_rc = cre2_match(re, text_buf, initial_text_len, 0, initial_text_len, CRE2_UNANCHORED, matches, 3);
    replace_rc = cre2_replace_re(re, &text_and_target, &rewrite);

    // step 5: Validate (combine results into checksum to ensure values are used)
    checksum = (long)pattern_len + (long)one_line_before + (long)one_line_after + (long)match_rc + (long)replace_rc + (long)(re != NULL) + (long)(set != NULL) + (long)matches[0].length + (long)text_and_target.length;
    (void)checksum;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}