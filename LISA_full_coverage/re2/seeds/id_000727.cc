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
//<ID> 727
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(world) (\\d+)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    char text_buf[] = "hello world 123 and world 456";
    const int text_len = (int)(sizeof(text_buf) - 1);
    char replace_target_buf[128];
    char extract_target_buf[128];
    const char rewrite_template[] = "[$1:$2]";
    cre2_options_t * opt = NULL;
    cre2_regexp_t * rex = NULL;
    cre2_set * set = NULL;
    cre2_string_t text_str;
    cre2_string_t text_and_target;
    cre2_string_t rewrite_str;
    cre2_string_t extract_target;
    int rc_add_simple = 0;
    int rc_compile = 0;
    int rc_extract = 0;
    int rc_replace = 0;
    cre2_encoding_t enc = CRE2_UNKNOWN;
    int computed_check = 0;

    // step 2: Setup
    memset(replace_target_buf, 0, sizeof(replace_target_buf));
    memset(extract_target_buf, 0, sizeof(extract_target_buf));
    opt = cre2_opt_new();
    cre2_opt_set_one_line(opt, 1);
    cre2_opt_set_case_sensitive(opt, 1);

    // step 3: Configure
    enc = cre2_opt_encoding(opt);
    rex = cre2_new(pattern, pattern_len, opt);
    set = cre2_set_new(opt, (cre2_anchor_t)0);
    rc_add_simple = cre2_set_add_simple(set, pattern);
    rc_compile = cre2_set_compile(set);

    // step 4: Operate
    text_str.data = text_buf;
    text_str.length = text_len;
    text_and_target.data = replace_target_buf;
    text_and_target.length = 0;
    rewrite_str.data = rewrite_template;
    rewrite_str.length = (int)(sizeof(rewrite_template) - 1);
    extract_target.data = extract_target_buf;
    extract_target.length = 0;
    rc_extract = cre2_extract_re(rex, &text_str, &rewrite_str, &extract_target);
    rc_replace = cre2_global_replace_re(rex, &text_and_target, &rewrite_str);

    // step 5: Validate
    computed_check = rc_add_simple + rc_compile + rc_extract + rc_replace + (int)enc + text_str.length + rewrite_str.length + extract_target.length;
    (void)computed_check;

    // step 6: Cleanup
    cre2_delete(rex);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}