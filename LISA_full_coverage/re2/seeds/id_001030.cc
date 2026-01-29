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
//<ID> 1030
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern1[] = "alpha([0-9]+)beta";
    const int pattern1_len = (int)(sizeof(pattern1) - 1);
    const char pattern2[] = "gamma";
    const int pattern2_len = (int)(sizeof(pattern2) - 1);
    char text_buf[] = "prefix alpha123beta middle gamma suffix";
    const int text_len = (int)(sizeof(text_buf) - 1);
    char min_buf[64];
    char max_buf[64];
    int match_indices[8];
    size_t match_len = 8;
    cre2_options_t * opt = NULL;
    cre2_set * set = NULL;
    cre2_regexp_t * re = NULL;
    cre2_string_t min_;
    cre2_string_t max_;
    size_t matched_count = 0;
    long computed_check = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    cre2_opt_set_case_sensitive(opt, 1);
    set = cre2_set_new(opt, CRE2_UNANCHORED);
    cre2_set_add_simple(set, pattern1);
    cre2_set_add_simple(set, pattern2);
    cre2_set_compile(set);
    re = cre2_new(pattern1, pattern1_len, opt);

    // step 3: Prepare buffers for possible match range and matches
    memset(min_buf, 0, sizeof(min_buf));
    memset(max_buf, 0, sizeof(max_buf));
    memset(match_indices, -1, sizeof(match_indices));
    min_.data = min_buf;
    min_.length = 0;
    max_.data = max_buf;
    max_.length = 0;

    // step 4: Core operations - compute possible match range for compiled regexp and run set match
    cre2_possible_match_range(re, &min_, &max_, 32);
    matched_count = cre2_set_match(set, text_buf, (size_t)text_len, match_indices, match_len);

    // step 5: Validate / compute lightweight checksum
    computed_check = (long)matched_count + (long)min_.length + (long)max_.length + (long)pattern1_len + (long)pattern2_len + (long)text_len;
    (void)computed_check;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}