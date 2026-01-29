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
//<ID> 662
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
    char text_buf[] = "prefix foobar suffix";
    const int text_len = (int)(sizeof(text_buf) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_named_groups_iter_t * iter = NULL;
    cre2_string_t match_arr[4];
    cre2_range_t ranges[4];
    int rc_easy = 0;
    int num_groups = 0;
    int computed_check = 0;

    // step 2: Setup
    memset(&match_arr, 0, sizeof(match_arr));
    memset(&ranges, 0, sizeof(ranges));
    opt = cre2_opt_new();
    cre2_opt_set_case_sensitive(opt, 1);

    // step 3: Configure
    re = cre2_new(pattern, pattern_len, opt);
    iter = cre2_named_groups_iter_new(re);
    num_groups = cre2_num_capturing_groups(re);

    // step 4: Operate
    rc_easy = cre2_easy_match(pattern, pattern_len, text_buf, text_len, match_arr, 4);
    cre2_strings_to_ranges(text_buf, ranges, match_arr, 4);

    // step 5: Validate
    computed_check = rc_easy + num_groups + (int)match_arr[0].length + (int)ranges[0].start + (int)ranges[0].past;
    (void)computed_check;

    // step 6: Cleanup
    cre2_named_groups_iter_delete(iter);
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}