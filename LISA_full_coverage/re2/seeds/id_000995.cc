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
//<ID> 995
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
    char text_buf[] = "alice123 bob456";
    const int text_len = (int)(sizeof(text_buf) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_named_groups_iter_t * iter = NULL;
    struct cre2_set * set = NULL;
    cre2_string_t match;
    cre2_string_t text_and_target;
    cre2_encoding_t enc = CRE2_UNKNOWN;
    int add_res = 0;
    int easy_match_res = 0;
    long computed_check = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    cre2_opt_set_encoding(opt, CRE2_UTF8);
    cre2_opt_set_case_sensitive(opt, 1);
    enc = cre2_opt_encoding(opt);

    // step 3: Operate
    re = cre2_new(pattern, pattern_len, opt);
    iter = cre2_named_groups_iter_new(re);
    set = cre2_set_new(opt, (cre2_anchor_t)0);
    add_res = cre2_set_add_simple(set, pattern);
    match.data = NULL;
    match.length = 0;
    text_and_target.data = text_buf;
    text_and_target.length = text_len;
    easy_match_res = cre2_easy_match(pattern, pattern_len, text_buf, text_len, &match, 1);

    // step 4: Validate / compute lightweight checksum
    computed_check = (long)add_res + (long)easy_match_res + (long)pattern_len + (long)text_and_target.length + (long)match.length + (long)enc;
    (void)computed_check;

    // step 5: Cleanup
    cre2_named_groups_iter_delete(iter);
    cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}