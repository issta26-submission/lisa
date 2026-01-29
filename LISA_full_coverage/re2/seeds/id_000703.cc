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
//<ID> 703
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
    const char text_buf[] = "xx foobar yy foo zz";
    const int text_len = (int)(sizeof(text_buf) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_set * set = NULL;
    int matches[8];
    const char * got_pattern = NULL;
    int rc_add = 0;
    int rc_compile = 0;
    size_t set_matches = 0;
    int num_groups = 0;
    int computed_check = 0;

    // step 2: Setup
    memset(matches, 0, sizeof(matches));
    opt = cre2_opt_new();
    cre2_opt_set_one_line(opt, 1);

    // step 3: Configure
    re = cre2_new(pattern, pattern_len, opt);
    set = cre2_set_new(opt, (cre2_anchor_t)0);
    rc_add = cre2_set_add_simple(set, pattern);
    rc_compile = cre2_set_compile(set);

    // step 4: Operate
    got_pattern = cre2_pattern(re);
    set_matches = cre2_set_match(set, text_buf, (size_t)text_len, matches, (size_t)(sizeof(matches)/sizeof(matches[0])));
    num_groups = cre2_num_capturing_groups(re);

    // step 5: Validate
    computed_check = (int)set_matches + rc_add + rc_compile + num_groups + (int)(got_pattern ? got_pattern[0] : 0) + matches[0];
    (void)computed_check;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}