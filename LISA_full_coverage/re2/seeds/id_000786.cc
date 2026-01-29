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
//<ID> 786
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "foo(bar)?";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    char text_buf[] = "xx foo bar yy";
    const int text_len = (int)(sizeof(text_buf) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_set * set = NULL;
    cre2_string_t match;
    int rc_easy = 0;
    int rc_set = 0;
    int num_groups = 0;
    const char * pat_from_re = NULL;
    int computed_check = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    cre2_opt_set_case_sensitive(opt, 1);
    re = cre2_new(pattern, pattern_len, opt);
    set = cre2_set_new(opt, CRE2_UNANCHORED);

    // step 3: Operate
    rc_set = cre2_set_add_simple(set, pattern);
    match.data = NULL;
    match.length = 0;
    rc_easy = cre2_easy_match(pattern, pattern_len, text_buf, text_len, &match, 1);
    num_groups = cre2_num_capturing_groups(re);
    pat_from_re = cre2_pattern(re);

    // step 4: Validate
    computed_check = rc_easy + rc_set + num_groups + (int)match.length + (int)pat_from_re[0] + (int)pattern[0] + cre2_opt_case_sensitive(opt);
    (void)computed_check;

    // step 5: Cleanup
    cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}