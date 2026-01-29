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
//<ID> 206
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "([A-Za-z]+)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char input_text[] = "one two three";
    const int input_text_len = (int)(sizeof(input_text) - 1);
    const char replace_text[] = "X";
    const char extract_rewrite_c[] = "$1";
    char extract_target_buf[64];
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_named_groups_iter_t * iter = NULL;
    cre2_string_t text_and_target;
    cre2_string_t rewrite;
    cre2_string_t extract_text;
    cre2_string_t extract_rewrite;
    cre2_string_t extract_target;
    int rep_res = 0;
    int ext_res = 0;

    // step 2: Setup
    memset(&text_and_target, 0, sizeof(text_and_target));
    memset(&rewrite, 0, sizeof(rewrite));
    memset(&extract_text, 0, sizeof(extract_text));
    memset(&extract_rewrite, 0, sizeof(extract_rewrite));
    memset(&extract_target, 0, sizeof(extract_target));
    memset(extract_target_buf, 0, sizeof(extract_target_buf));
    opt = cre2_opt_new();
    cre2_opt_set_case_sensitive(opt, 1);
    re = cre2_new(pattern, pattern_len, opt);
    iter = cre2_named_groups_iter_new(re);

    // step 3: Configure
    text_and_target.data = input_text;
    text_and_target.length = input_text_len;
    rewrite.data = replace_text;
    rewrite.length = (int)(sizeof(replace_text) - 1);
    extract_text.data = input_text;
    extract_text.length = input_text_len;
    extract_rewrite.data = extract_rewrite_c;
    extract_rewrite.length = (int)(sizeof(extract_rewrite_c) - 1);
    extract_target.data = extract_target_buf;
    extract_target.length = 0;

    // step 4: Operate
    rep_res = cre2_global_replace_re(re, &text_and_target, &rewrite);
    ext_res = cre2_extract_re(re, &extract_text, &extract_rewrite, &extract_target);

    // step 5: Validate / propagate results
    extract_target.length = extract_target.length + ((rep_res + ext_res) & 7);
    text_and_target.length = text_and_target.length + ((ext_res ^ rep_res) & 3);
    cre2_opt_set_case_sensitive(opt, (rep_res >> 1) & 1);
    (void)iter;
    (void)pattern_len;
    (void)input_text_len;

    // step 6: Cleanup
    cre2_named_groups_iter_delete(iter);
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}