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
//<ID> 765
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(foo)(bar)?";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    char text_buf[] = "xx foobar yy foo zz";
    const int text_len = (int)(sizeof(text_buf) - 1);
    char text_and_target_buf[128];
    char rewrite_buf[] = "REPLACED";
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_set * set = NULL;
    cre2_string_t text_and_target;
    cre2_string_t rewrite;
    const int MAX_NMATCH = 3;
    cre2_string_t easy_matches[MAX_NMATCH];
    int rc_easy = 0;
    int rc_replace = 0;
    int rc_add = 0;
    int rc_compile = 0;
    int num_groups = 0;
    cre2_encoding_t enc = CRE2_UNKNOWN;
    int computed_check = 0;

    // step 2: Setup / Configure
    memset(text_and_target_buf, 0, sizeof(text_and_target_buf));
    memcpy(text_and_target_buf, text_buf, (size_t)text_len);
    memset(easy_matches, 0, sizeof(easy_matches));
    opt = cre2_opt_new();
    cre2_opt_set_encoding(opt, CRE2_UTF8);
    re = cre2_new(pattern, pattern_len, opt);
    set = cre2_set_new(opt, CRE2_UNANCHORED);
    rc_add = cre2_set_add_simple(set, pattern);
    rc_compile = cre2_set_compile(set);

    // step 3: Operate
    rc_easy = cre2_easy_match(pattern, pattern_len, text_buf, text_len, easy_matches, MAX_NMATCH);
    num_groups = cre2_num_capturing_groups(re);
    text_and_target.data = text_and_target_buf;
    text_and_target.length = (int)strlen(text_and_target_buf);
    rewrite.data = rewrite_buf;
    rewrite.length = (int)(sizeof(rewrite_buf) - 1);
    rc_replace = cre2_replace_re(re, &text_and_target, &rewrite);

    // step 4: Validate
    enc = cre2_opt_encoding(opt);
    computed_check = rc_easy + rc_replace + rc_add + rc_compile + num_groups + (int)enc + text_and_target.length + rewrite.length + easy_matches[0].length + easy_matches[1].length + easy_matches[2].length;
    (void)computed_check;

    // step 5: Cleanup
    cre2_set_delete(set);
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}