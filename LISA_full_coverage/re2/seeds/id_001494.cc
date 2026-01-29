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
//<ID> 1494
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(?P<name>foo)bar";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    char text_buf[] = "zzz foobar yyy";
    const int text_len = (int)(sizeof(text_buf) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_named_groups_iter_t * iter = NULL;
    struct cre2_set * set = NULL;
    cre2_string_t match;
    int compile_ret = 0;
    int wb_flag_before = 0;
    int wb_flag_after = 0;
    long checksum = 0;

    // step 2: Setup
    memset(&match, 0, sizeof(match));
    opt = cre2_opt_new();
    wb_flag_before = cre2_opt_word_boundary(opt);

    // step 3: Configure
    cre2_opt_set_word_boundary(opt, 1);
    wb_flag_after = cre2_opt_word_boundary(opt);

    // step 4: Core operations
    re = cre2_new(pattern, pattern_len, opt);
    iter = cre2_named_groups_iter_new(re);
    set = cre2_set_new(opt, (cre2_anchor_t)0);
    cre2_set_add_simple(set, pattern);
    compile_ret = cre2_set_compile(set);

    // step 5: Validate
    checksum = (long)pattern_len + (long)text_len + (long)wb_flag_before + (long)wb_flag_after
               + (long)compile_ret + (long)(re != NULL) + (long)(iter != NULL) + (long)(set != NULL)
               + (long)(match.length);
    (void)checksum;

    // step 6: Cleanup
    cre2_named_groups_iter_delete(iter);
    cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}