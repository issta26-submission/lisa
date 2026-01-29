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
//<ID> 998
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(?P<name>[A-Za-z]+)([0-9]+)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char set_pattern[] = "foo|bar|baz";
    const int set_pattern_len = (int)(sizeof(set_pattern) - 1);
    char text_buf[] = "alice123 bob456 foo bar baz unknown";
    const int text_len = (int)(sizeof(text_buf) - 1);
    char rewrite_buf[128];
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_named_groups_iter_t * iter = NULL;
    struct cre2_set * set = NULL;
    cre2_string_t match;
    cre2_string_t text_and_target;
    cre2_string_t rewrite;
    cre2_encoding_t enc;
    int set_add_res = 0;
    int set_compile_res = 0;
    size_t set_match_res = 0;
    int set_match_indices[8];
    long computed_check = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    cre2_opt_set_case_sensitive(opt, 1);
    cre2_opt_set_perl_classes(opt, 1);
    cre2_opt_set_encoding(opt, CRE2_UTF8);
    enc = cre2_opt_encoding(opt);

    // step 3: Core operations - compile regexp, create iterator, create set and add pattern
    re = cre2_new(pattern, pattern_len, opt);
    iter = cre2_named_groups_iter_new(re);
    set = cre2_set_new(opt, (cre2_anchor_t)0);
    set_add_res = cre2_set_add_simple(set, set_pattern);
    set_compile_res = cre2_set_compile(set);

    // step 4: Operate - prepare strings, perform an easy match via set and a simple replace
    memset(rewrite_buf, 0, sizeof(rewrite_buf));
    match.data = NULL;
    match.length = 0;
    text_and_target.data = text_buf;
    text_and_target.length = text_len;
    rewrite.data = rewrite_buf;
    rewrite.length = 0;
    set_match_res = cre2_set_match(set, text_buf, text_len, set_match_indices, (size_t)(sizeof(set_match_indices) / sizeof(set_match_indices[0])));
    cre2_replace_re(re, &text_and_target, &rewrite);

    // step 5: Validate / compute lightweight checksum
    computed_check = (long)enc + (long)pattern_len + (long)set_add_res + (long)set_compile_res + (long)set_match_res + (long)text_and_target.length + (long)rewrite.length;
    (void)computed_check;

    // step 6: Cleanup
    cre2_named_groups_iter_delete(iter);
    cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}