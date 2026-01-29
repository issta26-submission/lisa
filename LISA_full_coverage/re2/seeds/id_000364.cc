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
//<ID> 364
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(^hello) (world)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char text[] = "hello world";
    const int text_len = (int)(sizeof(text) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_string_t input_text;
    cre2_string_t matches[3];
    int partial_result = 0;
    int match_result = 0;
    int opt_never_nl_flag = 0;
    const char * pattern_from_re = NULL;
    int computed_check = 0;

    // step 2: Setup
    memset(matches, 0, sizeof(matches));
    input_text.data = text;
    input_text.length = text_len;
    opt = cre2_opt_new();
    cre2_opt_set_never_nl(opt, 1);
    opt_never_nl_flag = cre2_opt_never_nl(opt);
    re = cre2_new(pattern, pattern_len, opt);
    pattern_from_re = cre2_pattern(re);

    // step 3: Operate
    partial_result = cre2_partial_match(pattern, &input_text, matches, 2);
    match_result = cre2_match(re, text, text_len, 0, text_len, CRE2_UNANCHORED, matches, 2);

    // step 4: Validate
    computed_check = partial_result + match_result + matches[0].length + matches[1].length + opt_never_nl_flag + (int)(pattern_from_re ? pattern_from_re[0] : 0);
    (void)computed_check;

    // step 5: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}