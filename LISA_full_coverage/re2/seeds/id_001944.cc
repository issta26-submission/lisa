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
//<ID> 1944
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(foo)(bar)?";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char text[] = "foobar baz";
    const int text_len = (int)(sizeof(text) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    int one_line_flag = 0;
    int num_groups = 0;
    int nmatch = 0;
    cre2_string_t * matches = NULL;
    int match_ret = 0;
    long checksum = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    cre2_opt_set_case_sensitive(opt, 1);
    one_line_flag = cre2_opt_one_line(opt);
    re = cre2_new(pattern, pattern_len, opt);

    // step 3: Core operations
    num_groups = cre2_num_capturing_groups(re);
    nmatch = num_groups + 1;
    matches = (cre2_string_t *)malloc(sizeof(cre2_string_t) * (size_t)nmatch);
    memset(matches, 0, sizeof(cre2_string_t) * (size_t)nmatch);
    match_ret = cre2_match(re, text, text_len, 0, text_len, CRE2_UNANCHORED, matches, nmatch);

    // step 4: Validate
    checksum = (long)pattern_len + (long)text_len + (long)one_line_flag + (long)num_groups + (long)match_ret + (long)(opt != NULL) + (long)(re != NULL) + (long)nmatch;
    (void)checksum;

    // step 5: Cleanup
    free(matches);
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}