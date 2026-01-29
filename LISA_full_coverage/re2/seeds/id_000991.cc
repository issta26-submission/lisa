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
//<ID> 991
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
    char text_buf[] = "alice123 bob456 alice789";
    const size_t text_len = (size_t)(sizeof(text_buf) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_set * set = NULL;
    cre2_named_groups_iter_t * iter = NULL;
    int add_res = 0;
    int set_compile_res = 0;
    size_t set_match_res = 0;
    int match_indices[8] = {0};
    int encoding_val = 0;
    long computed_check = 0;

    // step 2: Setup (Initialize -> Configure)
    opt = cre2_opt_new();
    cre2_opt_set_encoding(opt, CRE2_UTF8);
    encoding_val = (int)cre2_opt_encoding(opt);
    set = cre2_set_new(opt, (cre2_anchor_t)0);
    add_res = cre2_set_add_simple(set, pattern);
    set_compile_res = cre2_set_compile(set);
    re = cre2_new(pattern, pattern_len, opt);
    iter = cre2_named_groups_iter_new(re);

    // step 3: Operate
    set_match_res = cre2_set_match(set, text_buf, text_len, match_indices, (size_t)(sizeof(match_indices)/sizeof(match_indices[0])));

    // step 4: Validate / compute lightweight checksum
    computed_check = (long)pattern_len + (long)add_res + (long)set_compile_res + (long)set_match_res + (long)encoding_val + (long)match_indices[0];
    (void)computed_check;

    // step 5: Cleanup
    cre2_named_groups_iter_delete(iter);
    cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}