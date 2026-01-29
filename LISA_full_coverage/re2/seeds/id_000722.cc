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
//<ID> 722
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
    char replace_buf[] = "X$1Y";
    const int replace_len = (int)(sizeof(replace_buf) - 1);
    char work_buf[] = "foobar and foo";
    const int work_len = (int)(sizeof(work_buf) - 1);

    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_set * set = NULL;

    cre2_string_t text;
    cre2_string_t rewrite;
    cre2_string_t target;
    cre2_string_t text_and_target;

    int rc_add = 0;
    int rc_compile = 0;
    int rc_extract = 0;
    int rc_replace = 0;
    cre2_encoding_t enc = CRE2_UNKNOWN;
    int computed_check = 0;

    // step 2: Setup
    memset(&text, 0, sizeof(text));
    memset(&rewrite, 0, sizeof(rewrite));
    memset(&target, 0, sizeof(target));
    memset(&text_and_target, 0, sizeof(text_and_target));
    opt = cre2_opt_new();
    enc = cre2_opt_encoding(opt);

    // step 3: Configure
    re = cre2_new(pattern, pattern_len, opt);
    set = cre2_set_new(opt, (cre2_anchor_t)0);
    rc_add = cre2_set_add_simple(set, pattern);

    // step 4: Operate
    rc_compile = cre2_set_compile(set);
    text.data = text_buf;
    text.length = text_len;
    rewrite.data = replace_buf;
    rewrite.length = replace_len;
    target.data = NULL;
    target.length = 0;
    rc_extract = cre2_extract_re(re, &text, &rewrite, &target);
    text_and_target.data = work_buf;
    text_and_target.length = work_len;
    rc_replace = cre2_global_replace_re(re, &text_and_target, &rewrite);

    // step 5: Validate
    computed_check = rc_add + rc_compile + rc_extract + rc_replace + (int)enc + target.length + rewrite.length + text.length + text_and_target.length;
    (void)computed_check;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}