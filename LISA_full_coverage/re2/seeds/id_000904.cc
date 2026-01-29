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
//<ID> 904
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "^(hello)\\s+(world)$";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char set_pattern[] = "hello|world|foo";
    char text_buf[] = "xx hello world yy";
    const int text_len = (int)(sizeof(text_buf) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_set * set = NULL;
    cre2_named_groups_iter_t * iter = NULL;
    int word_boundary_flag = 0;
    int add_res = 0;
    int match_indices[4];
    size_t matched_count = 0;
    long computed_check = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    cre2_opt_set_word_boundary(opt, 1);
    word_boundary_flag = cre2_opt_word_boundary(opt);

    // step 3: Core operations
    re = cre2_new(pattern, pattern_len, opt);
    set = cre2_set_new(opt, CRE2_UNANCHORED);
    add_res = cre2_set_add_simple(set, set_pattern);
    memset(match_indices, 0, sizeof(match_indices));
    matched_count = cre2_set_match(set, text_buf, (size_t)text_len, match_indices, 4);
    iter = cre2_named_groups_iter_new(re);
    cre2_named_groups_iter_delete(iter);

    // step 4: Validate / compute lightweight checksum
    computed_check = (long)pattern_len + (long)word_boundary_flag + (long)add_res + (long)matched_count + (long)match_indices[0];
    (void)computed_check;

    // step 5: Cleanup
    cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}