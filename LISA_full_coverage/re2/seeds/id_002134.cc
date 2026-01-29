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
//<ID> 2134
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(?P<name>[A-Za-z]+)-(\\d+)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    char text_buf[] = "Alice-123";
    const int text_len = (int)(sizeof(text_buf) - 1);
    char rewrite_buf[] = "$1:ID$2";
    const int rewrite_len = (int)(sizeof(rewrite_buf) - 1);
    char tat_buf[] = "Bob-456 Carol-789";
    const int tat_len = (int)(sizeof(tat_buf) - 1);
    char target_buf[64];
    memset(target_buf, 0, sizeof(target_buf));
    cre2_string_t text = { text_buf, text_len };
    cre2_string_t rewrite = { rewrite_buf, rewrite_len };
    cre2_string_t target = { target_buf, 0 };
    cre2_string_t text_and_target = { tat_buf, tat_len };
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_named_groups_iter_t * iter = NULL;
    cre2_set * set = NULL;
    int set_add_ret = 0;
    int full_match_ret = 0;
    int replace_ret = 0;
    int64_t max_mem = 0;
    long checksum = 0;

    // step 2: Setup (options and set)
    opt = cre2_opt_new();
    cre2_opt_set_perl_classes(opt, 1);
    cre2_opt_set_case_sensitive(opt, 1);
    max_mem = cre2_opt_max_mem(opt);
    set = cre2_set_new(opt, (cre2_anchor_t)0);
    set_add_ret = cre2_set_add_simple(set, pattern);

    // step 3: Configure (compile regexp and prepare named-groups iterator)
    re = cre2_new(pattern, pattern_len, opt);
    iter = cre2_named_groups_iter_new(re);

    // step 4: Operate (perform a full match and a replace using the compiled regexp)
    full_match_ret = cre2_full_match(pattern, &text, &target, 1);
    replace_ret = cre2_replace_re(re, &text_and_target, &rewrite);

    // step 5: Validate (combine results so values flow through)
    checksum = (long)pattern_len + (long)text.length + (long)rewrite.length + (long)text_and_target.length + (long)max_mem + (long)set_add_ret + (long)full_match_ret + (long)replace_ret + (long)(opt != NULL) + (long)(re != NULL) + (long)(iter != NULL) + (long)(set != NULL) + (long)target.length;
    (void)checksum;

    // step 6: Cleanup (release resources)
    cre2_named_groups_iter_delete(iter);
    cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}