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
//<ID> 836
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "foo(bar)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    char text_buf[] = "xx foo(bar) zz";
    const size_t text_len = (size_t)(sizeof(text_buf) - 1);
    cre2_options_t * opt = NULL;
    cre2_set * set = NULL;
    cre2_regexp_t * re = NULL;
    cre2_named_groups_iter_t * iter = NULL;
    cre2_encoding_t enc = CRE2_UNKNOWN;
    int matches[4];
    size_t set_matches = 0;
    long computed_check = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    cre2_opt_set_case_sensitive(opt, 1);
    enc = cre2_opt_encoding(opt);
    set = cre2_set_new(opt, CRE2_UNANCHORED);

    // step 3: Operate
    memset(matches, 0, sizeof(matches));
    cre2_set_add_simple(set, pattern);
    cre2_set_compile(set);
    set_matches = cre2_set_match(set, text_buf, text_len, matches, 4);
    re = cre2_new(pattern, pattern_len, opt);
    iter = cre2_named_groups_iter_new(re);

    // step 4: Additional operations
    (void)set_matches;
    (void)matches[0];

    // step 5: Validate
    computed_check = (long)enc + (long)set_matches + (long)matches[0] + (long)text_len + (long)(pattern[0]);
    (void)computed_check;

    // step 6: Cleanup
    cre2_named_groups_iter_delete(iter);
    cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}