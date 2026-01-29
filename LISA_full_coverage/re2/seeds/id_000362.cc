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
//<ID> 362
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(he)llo";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    char text[] = "hello world";
    const int text_len = (int)(sizeof(text) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_string_t input;
    cre2_string_t matches[2];
    cre2_string_t partial_match;
    const char * pattern_from_re = NULL;
    int never_nl_flag = 0;
    int partial_res = 0;
    int match_res = 0;
    int computed_check = 0;

    // step 2: Setup
    memset(matches, 0, sizeof(matches));
    partial_match.data = NULL;
    partial_match.length = 0;
    input.data = text;
    input.length = text_len;
    opt = cre2_opt_new();
    never_nl_flag = cre2_opt_never_nl(opt);

    // step 3: Operate
    re = cre2_new(pattern, pattern_len, opt);
    pattern_from_re = cre2_pattern(re);
    partial_res = cre2_partial_match(pattern_from_re, &input, &partial_match, 1);
    match_res = cre2_match(re, text, text_len, 0, text_len, CRE2_UNANCHORED, matches, 2);

    // step 4: Validate
    computed_check = partial_res + match_res + never_nl_flag + matches[0].length + partial_match.length;
    (void)computed_check;

    // step 5: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}