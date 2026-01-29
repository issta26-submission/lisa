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
//<ID> 862
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(foo)(bar)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    char text_buf[] = "xx foobar yy";
    const int text_len = (int)(sizeof(text_buf) - 1);
    cre2_options_t * opt = NULL;
    cre2_set * set = NULL;
    cre2_regexp_t * re = NULL;
    cre2_string_t match_out[3];
    cre2_string_t text_and_target;
    cre2_string_t rewrite;
    int set_add_res = 0;
    int compile_res = 0;
    int easy_res = 0;
    int replace_res = 0;
    int case_flag = 0;
    long computed_check = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    cre2_opt_set_case_sensitive(opt, 1);
    case_flag = cre2_opt_case_sensitive(opt);
    set = cre2_set_new(opt, CRE2_UNANCHORED);

    // step 3: Operate - add pattern to set and compile, then easy match and create regexp for replace
    set_add_res = cre2_set_add_simple(set, pattern);
    compile_res = cre2_set_compile(set);
    memset(match_out, 0, sizeof(match_out));
    easy_res = cre2_easy_match(pattern, pattern_len, text_buf, text_len, match_out, 3);
    re = cre2_new(pattern, pattern_len, opt);
    memset(&text_and_target, 0, sizeof(text_and_target));
    memset(&rewrite, 0, sizeof(rewrite));
    text_and_target.data = text_buf;
    text_and_target.length = text_len;
    rewrite.data = "X";
    rewrite.length = 1;
    replace_res = cre2_replace_re(re, &text_and_target, &rewrite);

    // step 4: Validate / compute lightweight checksum
    computed_check = (long)set_add_res + (long)compile_res + (long)easy_res + (long)replace_res + (long)match_out[0].length + (long)case_flag;
    (void)computed_check;

    // step 5: Cleanup
    cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}