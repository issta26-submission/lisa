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
//<ID> 2216
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
    char text_buf[] = "Alice-123 Bob-456 Carol-789";
    const int text_len = (int)(sizeof(text_buf) - 1);
    char rewrite_buf[] = "$name:[$num]";
    const int rewrite_len = (int)(sizeof(rewrite_buf) - 1);
    cre2_options_t * opt = NULL;
    cre2_set * set = NULL;
    cre2_regexp_t * re = NULL;
    cre2_named_groups_iter_t * iter = NULL;
    cre2_string_t text_and_target;
    cre2_string_t rewrite;
    const char * found_name = NULL;
    int found_index = -1;
    bool iter_next_ret = false;
    int add_ret = 0;
    int compile_ret = 0;
    int replace_ret = 0;
    long checksum = 0;

    // step 2: Setup
    memset(&text_and_target, 0, sizeof(text_and_target));
    memset(&rewrite, 0, sizeof(rewrite));
    opt = cre2_opt_new();
    set = cre2_set_new(opt, (cre2_anchor_t)0);
    add_ret = cre2_set_add_simple(set, pattern);
    compile_ret = cre2_set_compile(set);

    // step 3: Core operations - create regexp and prepare strings
    re = cre2_new(pattern, pattern_len, opt);
    text_and_target.data = text_buf;
    text_and_target.length = text_len;
    rewrite.data = rewrite_buf;
    rewrite.length = rewrite_len;

    // step 4: Named groups iteration (single step without loop)
    iter = cre2_named_groups_iter_new(re);
    iter_next_ret = cre2_named_groups_iter_next(iter, &found_name, &found_index);

    // step 5: Perform a global replace using the compiled regexp
    replace_ret = cre2_global_replace_re(re, &text_and_target, &rewrite);

    // step 6: Cleanup
    cre2_named_groups_iter_delete(iter);
    cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    checksum = (long)pattern_len + (long)text_len + (long)compile_ret + (long)replace_ret + (long)iter_next_ret + (long)found_index + (long)(found_name != NULL) + (long)(text_and_target.length);
    (void)checksum;

    // API sequence test completed successfully
    return 66;
}