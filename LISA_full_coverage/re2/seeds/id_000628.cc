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
//<ID> 628
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "h(ello) world";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char text_buf[] = "hello world";
    const int text_len = (int)(sizeof(text_buf) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_string_t strings[2];
    cre2_range_t ranges[2];
    int nmatch = 2;
    int literal_flag = 1;
    int cs_flag = 0;
    cre2_encoding_t enc = CRE2_UNKNOWN;
    int n_capturing = 0;
    int rc_easy = 0;
    int computed_check = 0;

    // step 2: Setup
    memset(strings, 0, sizeof(strings));
    memset(ranges, 0, sizeof(ranges));
    opt = cre2_opt_new();
    cre2_opt_set_literal(opt, literal_flag);
    cre2_opt_set_case_sensitive(opt, 1);
    cs_flag = cre2_opt_case_sensitive(opt);

    // step 3: Configure
    re = cre2_new(pattern, pattern_len, opt);
    n_capturing = cre2_num_capturing_groups(re);

    // step 4: Operate
    strings[0].data = text_buf;
    strings[0].length = 5;             // "hello"
    strings[1].data = text_buf + 6;
    strings[1].length = 5;             // "world"
    cre2_strings_to_ranges(text_buf, ranges, strings, nmatch);
    rc_easy = cre2_easy_match(pattern, pattern_len, text_buf, text_len, strings, nmatch);

    // step 5: Validate
    enc = cre2_opt_encoding(opt);
    computed_check = cs_flag + literal_flag + n_capturing + (int)ranges[0].start + (int)ranges[0].past + (int)ranges[1].start + (int)ranges[1].past + (int)enc + rc_easy;
    (void)computed_check;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}