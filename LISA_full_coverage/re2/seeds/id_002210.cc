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
//<ID> 2210
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(?P<name>[A-Za-z]+)-(?P<num>[0-9]+)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    char text_buf[] = "Users: Alice-123; Bob-456; Carol-789";
    const int text_len = (int)(sizeof(text_buf) - 1);
    char rewrite_buf[] = "<NAME:$1><NUM:$2>";
    const int rewrite_len = (int)(sizeof(rewrite_buf) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_named_groups_iter_t * iter = NULL;
    cre2_set * set = NULL;
    cre2_string_t text_and_target;
    cre2_string_t rewrite;
    const char * found_name = NULL;
    int found_index = 0;
    bool iter_got = false;
    int add_ret1 = 0;
    int compile_ret = 0;
    int replace_ret = 0;
    long checksum = 0;

    // step 2: Setup
    memset(&text_and_target, 0, sizeof(text_and_target));
    memset(&rewrite, 0, sizeof(rewrite));
    opt = cre2_opt_new();
    cre2_opt_set_case_sensitive(opt, 1);
    cre2_opt_set_perl_classes(opt, 1);
    text_and_target.data = text_buf;
    text_and_target.length = text_len;
    rewrite.data = rewrite_buf;
    rewrite.length = rewrite_len;

    // step 3: Initialize regex and set, add patterns
    re = cre2_new(pattern, pattern_len, opt);
    set = cre2_set_new(opt, (cre2_anchor_t)0);
    add_ret1 = cre2_set_add_simple(set, pattern);

    // step 4: Compile the set
    compile_ret = cre2_set_compile(set);

    // step 5: Operate with named-groups iterator and perform global replace with compiled regex
    iter = cre2_named_groups_iter_new(re);
    iter_got = cre2_named_groups_iter_next(iter, &found_name, &found_index);
    replace_ret = cre2_global_replace_re(re, &text_and_target, &rewrite);
    cre2_named_groups_iter_delete(iter);

    // step 6: Validate and Cleanup
    checksum = (long)pattern_len + (long)text_len + (long)rewrite_len + (long)add_ret1 + (long)compile_ret + (long)replace_ret + (long)iter_got + (long)found_index + (long)(found_name != NULL) + (long)(opt != NULL) + (long)(re != NULL) + (long)(set != NULL) + (long)text_and_target.length;
    (void)checksum;
    cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}