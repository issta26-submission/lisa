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
//<ID> 365
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "ab(c|d)+e?";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    char text_buf[] = "xxabccdeyy";
    const int text_len = (int)(sizeof(text_buf) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_string_t text;
    cre2_string_t matches[4];
    const char * pattern_from_re = NULL;
    int opt_flag = 0;
    int match_result = 0;
    int partial_result = 0;
    int computed_check = 0;

    // step 2: Setup
    memset(matches, 0, sizeof(matches));
    text.data = text_buf;
    text.length = text_len;
    opt = cre2_opt_new();

    // step 3: Configure
    cre2_opt_set_never_nl(opt, 1);
    opt_flag = cre2_opt_never_nl(opt);

    // step 4: Operate
    re = cre2_new(pattern, pattern_len, opt);
    pattern_from_re = cre2_pattern(re);
    match_result = cre2_match(re, text_buf, text_len, 0, text_len, CRE2_UNANCHORED, matches, 4);
    partial_result = cre2_partial_match(pattern_from_re, &text, &matches[1], 3);

    // step 5: Validate
    computed_check = opt_flag + match_result + partial_result + matches[0].length + matches[1].length;
    (void)computed_check;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}