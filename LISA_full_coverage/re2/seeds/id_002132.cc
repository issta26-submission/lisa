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
//<ID> 2132
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
    char source_buf[] = "Alice-1234";
    const int source_len = (int)(sizeof(source_buf) - 1);
    char text_and_target_buf[] = "Alice-1234 and Bob-5678";
    const int tat_len = (int)(sizeof(text_and_target_buf) - 1);
    char rewrite_buf[] = "$1:repl";
    const int rewrite_len = (int)(sizeof(rewrite_buf) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_named_groups_iter_t * iter = NULL;
    cre2_set * set = NULL;
    cre2_string_t text = { source_buf, source_len };
    cre2_string_t text_and_target = { text_and_target_buf, tat_len };
    cre2_string_t rewrite = { rewrite_buf, rewrite_len };
    cre2_string_t matches[3];
    memset(matches, 0, sizeof(matches));
    int full_ret = 0;
    int replace_ret = 0;
    int add_simple_ret = 0;
    int64_t maxmem = 0;
    long checksum = 0;

    // step 2: Setup (create and configure options)
    opt = cre2_opt_new();
    cre2_opt_set_perl_classes(opt, 1);
    cre2_opt_set_case_sensitive(opt, 1);
    maxmem = cre2_opt_max_mem(opt);

    // step 3: Configure (compile regexp)
    re = cre2_new(pattern, pattern_len, opt);

    // step 4: Prepare named-groups iterator and a set, add simple pattern
    iter = cre2_named_groups_iter_new(re);
    set = cre2_set_new(opt, (cre2_anchor_t)0);
    add_simple_ret = cre2_set_add_simple(set, pattern);

    // step 5: Operate (full match against a source string and perform a replace on text_and_target), Validate (combine values)
    full_ret = cre2_full_match(pattern, &text, matches, 3);
    replace_ret = cre2_replace_re(re, &text_and_target, &rewrite);
    checksum = (long)pattern_len + (long)full_ret + (long)replace_ret + (long)add_simple_ret + (long)maxmem + (long)matches[0].length + (long)text.length + (long)text_and_target.length + (long)(re != NULL) + (long)(opt != NULL) + (long)(set != NULL) + (long)(iter != NULL);
    (void)checksum;

    // step 6: Cleanup (release resources)
    cre2_named_groups_iter_delete(iter);
    cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}