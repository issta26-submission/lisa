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
//<ID> 1380
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(?P<g>hello)|world";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_set * set = NULL;
    cre2_named_groups_iter_t * iter = NULL;
    const char * found_name = NULL;
    int found_index = -1;
    cre2_encoding_t encoding = CRE2_UNKNOWN;
    int literal_flag = 0;
    bool iter_next_ret = false;
    long computed_check = 0;

    // step 2: Setup
    memset(&found_name, 0, sizeof(found_name));
    memset(&found_index, 0, sizeof(found_index));
    memset(&computed_check, 0, sizeof(computed_check));
    opt = cre2_opt_new();

    // step 3: Configure
    cre2_opt_set_literal(opt, 1);
    literal_flag = cre2_opt_literal(opt);
    encoding = cre2_opt_encoding(opt);

    // step 4: Core operations
    set = cre2_set_new(opt, CRE2_UNANCHORED);
    re = cre2_new(pattern, pattern_len, opt);
    iter = cre2_named_groups_iter_new(re);
    iter_next_ret = cre2_named_groups_iter_next(iter, &found_name, &found_index);

    // step 5: Validate / compute lightweight checksum
    computed_check = (long)pattern_len + (long)literal_flag + (long)encoding
                     + (long)(re != NULL) + (long)(set != NULL)
                     + (long)found_index + (long)(found_name != NULL)
                     + (long)iter_next_ret;
    (void)computed_check;

    // step 6: Cleanup
    cre2_named_groups_iter_delete(iter);
    cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}