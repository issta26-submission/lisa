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
//<ID> 990
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(?P<word>[A-Za-z]+)([0-9]+)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char set_pattern[] = "[a-z]+";
    const int set_pattern_len = (int)(sizeof(set_pattern) - 1);
    char text_buf[] = "alice123 bob456 carol789";
    const int text_len = (int)(sizeof(text_buf) - 1);
    char rewrite_buf[128];
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_named_groups_iter_t * iter = NULL;
    struct cre2_set * set = NULL;
    cre2_string_t text_and_target;
    cre2_string_t rewrite;
    cre2_string_t match;
    cre2_encoding_t enc = CRE2_UNKNOWN;
    int match_arr[8];
    size_t set_match_count = 0;
    long computed_check = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    cre2_opt_set_case_sensitive(opt, 1);
    cre2_opt_set_encoding(opt, CRE2_UTF8);
    enc = cre2_opt_encoding(opt);

    // step 3: Core operations - compile regexp, create named group iterator, build a set and match it
    re = cre2_new(pattern, pattern_len, opt);
    iter = cre2_named_groups_iter_new(re);
    set = cre2_set_new(opt, (cre2_anchor_t)0);
    cre2_set_add_simple(set, set_pattern);
    cre2_set_compile(set);

    // step 4: Operate on data buffers
    memset(rewrite_buf, 0, sizeof(rewrite_buf));
    match.data = NULL;
    match.length = 0;
    text_and_target.data = text_buf;
    text_and_target.length = text_len;
    rewrite.data = rewrite_buf;
    rewrite.length = 0;
    set_match_count = cre2_set_match(set, text_buf, (size_t)text_len, match_arr, (size_t)(sizeof(match_arr)/sizeof(match_arr[0])));

    // step 5: Validate / compute lightweight checksum
    computed_check = (long)pattern_len + (long)set_pattern_len + (long)text_and_target.length + (long)set_match_count + (long)enc + (long)rewrite.length;
    (void)computed_check;

    // step 6: Cleanup
    cre2_named_groups_iter_delete(iter);
    cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}