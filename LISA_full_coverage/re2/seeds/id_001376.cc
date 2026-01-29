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
//<ID> 1376
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(?P<name>foo)|bar";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    char text_buf[] = "xxfooyybarzz";
    const int text_len = (int)(sizeof(text_buf) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_named_groups_iter_t * iter = NULL;
    const char * pattern_from_re = NULL;
    cre2_string_t matches[4];
    int match_ret = 0;
    int n_groups = 0;
    int literal_flag = 0;
    int version_cur = 0;
    int64_t max_mem_val = 0;
    long computed_check = 0;

    // step 2: Setup
    memset(matches, 0, sizeof(matches));
    opt = cre2_opt_new();

    // step 3: Configure
    cre2_opt_set_literal(opt, 1);
    cre2_opt_set_max_mem(opt, (int64_t)4096);
    literal_flag = cre2_opt_literal(opt);
    max_mem_val = cre2_opt_max_mem(opt);
    version_cur = cre2_version_interface_current();

    // step 4: Core operations
    re = cre2_new(pattern, pattern_len, opt);
    pattern_from_re = cre2_pattern(re);
    n_groups = cre2_num_capturing_groups(re);
    iter = cre2_named_groups_iter_new(re);
    match_ret = cre2_match(re, text_buf, text_len, 0, text_len, CRE2_UNANCHORED, matches, 4);

    // step 5: Validate
    computed_check = (long)pattern_len + (long)text_len + (long)version_cur
                     + (long)literal_flag + (long)max_mem_val + (long)n_groups
                     + (long)match_ret + (long)(re != NULL) + (long)(iter != NULL)
                     + (long)(pattern_from_re != NULL);
    (void)computed_check;
    (void)matches;

    // step 6: Cleanup
    cre2_named_groups_iter_delete(iter);
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}