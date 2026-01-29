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
//<ID> 200
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(a)(b)?";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    char input_text[] = "prefix ab suffix";
    const int input_text_len = (int)(sizeof(input_text) - 1);
    char extract_text[] = "ab";
    const int extract_text_len = (int)(sizeof(extract_text) - 1);
    char replace_buffer[] = "prefix ab suffix";
    const int replace_buffer_len = (int)(sizeof(replace_buffer) - 1);
    char target_buf[64];
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_named_groups_iter_t * iter = NULL;
    cre2_string_t text_and_target;
    cre2_string_t rewrite;
    cre2_string_t text;
    cre2_string_t target;
    int res_extract = 0;
    int res_replace = 0;

    // step 2: Setup
    memset(&text_and_target, 0, sizeof(text_and_target));
    memset(&rewrite, 0, sizeof(rewrite));
    memset(&text, 0, sizeof(text));
    memset(&target, 0, sizeof(target));
    opt = cre2_opt_new();
    cre2_opt_set_case_sensitive(opt, 1);
    cre2_opt_set_encoding(opt, CRE2_UTF8);
    re = cre2_new(pattern, pattern_len, opt);
    iter = cre2_named_groups_iter_new(re);

    // step 3: Configure
    text_and_target.data = replace_buffer;
    text_and_target.length = replace_buffer_len;
    rewrite.data = "$1Z";
    rewrite.length = 3;
    text.data = extract_text;
    text.length = extract_text_len;
    target.data = target_buf;
    target.length = 0;

    // step 4: Operate
    res_extract = cre2_extract_re(re, &text, &rewrite, &target);
    res_replace = cre2_global_replace_re(re, &text_and_target, &rewrite);

    // step 5: Validate / propagate results
    target.length = target.length + (res_extract & 7);
    text_and_target.length = text_and_target.length + (res_replace & 7);
    (void)input_text_len;
    (void)pattern_len;
    (void)iter;

    // step 6: Cleanup
    cre2_named_groups_iter_delete(iter);
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}