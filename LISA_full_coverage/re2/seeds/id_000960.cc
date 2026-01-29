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
//<ID> 960
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char set_pattern[] = "foo|bar";
    const char re_pattern[] = "(?P<name>[A-Za-z]+)([0-9]+)";
    const char text_buf[] = "foo123 baz789 bar456";
    const char rewrite_buf[] = "$1-REPL-$2";
    const int text_len = (int)(sizeof(text_buf) - 1);
    const int rewrite_len = (int)(sizeof(rewrite_buf) - 1);
    cre2_options_t * opt = NULL;
    cre2_set * set = NULL;
    cre2_regexp_t * re = NULL;
    cre2_named_groups_iter_t * iter = NULL;
    cre2_string_t text_and_target;
    cre2_string_t rewrite;
    cre2_string_t scratch[2];
    int add_res = 0;
    int compile_res = 0;
    int replace_res = 0;
    int num_groups = 0;
    long computed_check = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    cre2_opt_set_encoding(opt, CRE2_UTF8);
    set = cre2_set_new(opt, CRE2_UNANCHORED);
    add_res = cre2_set_add_simple(set, set_pattern);
    compile_res = cre2_set_compile(set);
    re = cre2_new(re_pattern, (int)(sizeof(re_pattern) - 1), opt);

    // step 3: Core operations
    text_and_target.data = text_buf;
    text_and_target.length = text_len;
    rewrite.data = rewrite_buf;
    rewrite.length = rewrite_len;
    memset(scratch, 0, sizeof(scratch));
    replace_res = cre2_replace_re(re, &text_and_target, &rewrite);
    num_groups = cre2_num_capturing_groups(re);
    iter = cre2_named_groups_iter_new(re);

    // step 4: Validate / compute lightweight checksum
    computed_check = (long)replace_res + (long)num_groups + (long)add_res + (long)compile_res + (long)text_and_target.length + (long)rewrite.length + (long)text_and_target.data[0];
    (void)computed_check;

    // step 5: Cleanup
    cre2_named_groups_iter_delete(iter);
    cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}