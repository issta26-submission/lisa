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
//<ID> 2138
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
    int text_len = (int)(sizeof(text_buf) - 1);
    char tat_buf[] = "Bob-42 and Charlie-7";
    int tat_len = (int)(sizeof(tat_buf) - 1);
    char rewrite_buf[] = "$1_REPLACED";
    int rewrite_len = (int)(sizeof(rewrite_buf) - 1);
    cre2_options_t *opt = NULL;
    cre2_regexp_t *re = NULL;
    cre2_set *set = NULL;
    cre2_named_groups_iter_t *iter = NULL;
    cre2_string_t text = { text_buf, text_len };
    cre2_string_t text_and_target = { tat_buf, tat_len };
    cre2_string_t rewrite = { rewrite_buf, rewrite_len };
    cre2_string_t matches[4];
    int full_ret = 0;
    int add_ret = 0;
    int replace_ret = 0;
    int64_t maxmem = 0;
    long checksum = 0;

    // step 2: Setup (create and configure options)
    opt = cre2_opt_new();
    cre2_opt_set_perl_classes(opt, 1);
    cre2_opt_set_case_sensitive(opt, 1);

    // step 3: Configure (compile regexp, create named-group iterator and set)
    re = cre2_new(pattern, pattern_len, opt);
    iter = cre2_named_groups_iter_new(re);
    set = cre2_set_new(opt, (cre2_anchor_t)0);
    add_ret = cre2_set_add_simple(set, pattern);
    maxmem = cre2_opt_max_mem(opt);

    // step 4: Operate (prepare match buffers and perform full match and replace)
    memset(matches, 0, sizeof(matches));
    full_ret = cre2_full_match(pattern, &text, matches, 3);
    replace_ret = cre2_replace_re(re, &text_and_target, &rewrite);

    // step 5: Validate (combine outcomes so values flow through)
    checksum = (long)pattern_len + (long)maxmem + (long)add_ret + (long)full_ret + (long)replace_ret + (long)text.length + (long)text_and_target.length + (long)rewrite.length + (long)matches[0].length + (long)(re != NULL) + (long)(set != NULL) + (long)(opt != NULL) + (long)(iter != NULL);
    (void)checksum;

    // step 6: Cleanup (release resources)
    cre2_named_groups_iter_delete(iter);
    cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}