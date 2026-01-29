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
//<ID> 2133
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // Step 1: Declarations
    const char pattern[] = "(?P<name>[A-Za-z]+)-(\\d+)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    char input_buf[] = "Alice-42";
    cre2_string_t input = { input_buf, (int)(sizeof(input_buf) - 1) };
    char text_and_target_buf[] = "Alice-42 Bob-007";
    cre2_string_t text_and_target = { text_and_target_buf, (int)(sizeof(text_and_target_buf) - 1) };
    char rewrite_buf[] = "$1:$2";
    cre2_string_t rewrite = { rewrite_buf, (int)(sizeof(rewrite_buf) - 1) };
    cre2_string_t matches[4];
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_named_groups_iter_t * iter = NULL;
    cre2_set * set = NULL;
    int full_ret = 0;
    int replace_ret = 0;
    int set_add_ret = 0;
    int64_t maxmem = 0;
    long checksum = 0;

    // Step 2: Setup (create and configure options)
    opt = cre2_opt_new();
    cre2_opt_set_perl_classes(opt, 1);
    cre2_opt_set_case_sensitive(opt, 1);
    cre2_opt_set_max_mem(opt, (int64_t)(4 * 1024 * 1024));
    maxmem = cre2_opt_max_mem(opt);

    // Step 3: Configure (compile regexp, create iterator and set)
    re = cre2_new(pattern, pattern_len, opt);
    iter = cre2_named_groups_iter_new(re);
    set = cre2_set_new(opt, (cre2_anchor_t)0);

    // Step 4: Operate (prepare matches and perform full match, replace, and add to set)
    memset(matches, 0, sizeof(matches));
    full_ret = cre2_full_match(pattern, &input, matches, 3);
    replace_ret = cre2_replace_re(re, &text_and_target, &rewrite);
    set_add_ret = cre2_set_add_simple(set, pattern);

    // Step 5: Validate (combine values so results flow through)
    checksum = (long)pattern_len + (long)full_ret + (long)replace_ret + (long)set_add_ret + (long)text_and_target.length + (long)input.length + (long)rewrite.length + (long)maxmem + (long)(re != NULL) + (long)(opt != NULL) + (long)(iter != NULL) + (long)(set != NULL);
    (void)checksum;

    // Step 6: Cleanup (release resources)
    cre2_named_groups_iter_delete(iter);
    cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}