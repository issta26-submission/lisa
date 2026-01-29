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
//<ID> 2136
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
    char input_buf[] = "Alice-123";
    int input_len = (int)(sizeof(input_buf) - 1);
    char match0_buf[64];
    char match1_buf[64];
    cre2_string_t input = { input_buf, input_len };
    cre2_string_t matches[3];
    cre2_string_t rewrite;
    char rewrite_buf[] = "$1#$2";
    cre2_string_t text_and_target;
    char tat_buf[] = "Alice-123 Bob-456";
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_set * set = NULL;
    cre2_named_groups_iter_t * iter = NULL;
    int full_ret = 0;
    int replace_ret = 0;
    int add_simple_ret = 0;
    int64_t maxmem = 0;
    long checksum = 0;

    // step 2: Setup (options and buffers)
    opt = cre2_opt_new();
    cre2_opt_set_perl_classes(opt, 1);
    cre2_opt_set_case_sensitive(opt, 1);
    maxmem = cre2_opt_max_mem(opt);
    memset(match0_buf, 0, sizeof(match0_buf));
    memset(match1_buf, 0, sizeof(match1_buf));
    matches[0].data = match0_buf;
    matches[0].length = 0;
    matches[1].data = match1_buf;
    matches[1].length = 0;
    matches[2].data = NULL;
    matches[2].length = 0;
    memset(rewrite_buf, 0, sizeof(rewrite_buf)); /* clear then fill */
    memcpy(rewrite_buf, "$1-$2", sizeof("$1-$2"));
    rewrite.data = rewrite_buf;
    rewrite.length = (int)(sizeof("$1-$2") - 1);
    memset(tat_buf, 0, sizeof(tat_buf));
    memcpy(tat_buf, "Alice-123 Bob-456", (size_t)(sizeof("Alice-123 Bob-456") - 1));
    text_and_target.data = tat_buf;
    text_and_target.length = (int)(sizeof("Alice-123 Bob-456") - 1);

    // step 3: Configure (compile regexp and create set)
    re = cre2_new(pattern, pattern_len, opt);
    set = cre2_set_new(opt, (cre2_anchor_t)0);
    add_simple_ret = cre2_set_add_simple(set, pattern);
    iter = cre2_named_groups_iter_new(re);

    // step 4: Operate (perform a full match and a replace using compiled regexp)
    full_ret = cre2_full_match(pattern, &input, matches, 3);
    replace_ret = cre2_replace_re(re, &text_and_target, &rewrite);

    // step 5: Validate (combine results so values flow through)
    checksum = (long)pattern_len + (long)input.length + (long)matches[0].length + (long)matches[1].length + (long)full_ret + (long)replace_ret + (long)add_simple_ret + (long)maxmem + (long)(re != NULL) + (long)(opt != NULL) + (long)(set != NULL) + (long)(iter != NULL) + (long)text_and_target.length;
    (void)checksum;

    // step 6: Cleanup (release resources)
    cre2_named_groups_iter_delete(iter);
    cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    /* API sequence test completed successfully */
    return 66;
}