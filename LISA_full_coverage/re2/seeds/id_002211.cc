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
//<ID> 2211
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "([A-Za-z]+)-([0-9]+)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    char text_buf[] = "Alice-123 Bob-456 Carol-789";
    const int text_len = (int)(sizeof(text_buf) - 1);
    char rewrite_buf[] = "Name:$1#Id:$2";
    const int rewrite_len = (int)(sizeof(rewrite_buf) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    struct cre2_named_groups_iter_t * iter = NULL;
    cre2_set * set = NULL;
    cre2_string_t text_and_target;
    cre2_string_t rewrite;
    const char * found_name = NULL;
    int found_index = -1;
    bool has_name = false;
    int add_ret = 0;
    int comp_ret = 0;
    int rep_ret = 0;
    long checksum = 0;

    // step 2: Setup
    memset(&text_and_target, 0, sizeof(text_and_target));
    memset(&rewrite, 0, sizeof(rewrite));
    text_and_target.data = text_buf;
    text_and_target.length = text_len;
    rewrite.data = rewrite_buf;
    rewrite.length = rewrite_len;
    opt = cre2_opt_new();
    re = cre2_new(pattern, pattern_len, opt);
    set = cre2_set_new(opt, (cre2_anchor_t)0);

    // step 3: Configure
    add_ret = cre2_set_add_simple(set, "([A-Za-z]+)-([0-9]+)");
    comp_ret = cre2_set_compile(set);
    iter = cre2_named_groups_iter_new(re);

    // step 4: Operate
    has_name = cre2_named_groups_iter_next(iter, &found_name, &found_index);
    rep_ret = cre2_global_replace_re(re, &text_and_target, &rewrite);

    // step 5: Validate
    checksum = (long)pattern_len + (long)text_and_target.length + (long)rewrite.length + (long)add_ret + (long)comp_ret + (long)rep_ret + (long)has_name + (long)found_index + (long)(opt != NULL) + (long)(re != NULL) + (long)(set != NULL) + (long)(found_name != NULL);
    (void)checksum;

    // step 6: Cleanup
    cre2_named_groups_iter_delete(iter);
    cre2_set_delete(set);
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}