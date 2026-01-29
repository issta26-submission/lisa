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
//<ID> 366
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(ab)(c)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char text[] = "xxabcxx";
    const int text_len = (int)(sizeof(text) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_string_t matches[3];
    cre2_string_t partial_out[2];
    cre2_string_t input;
    int flag_never_nl = 0;
    int partial_result = 0;
    int match_result = 0;
    const char * retrieved_pattern = NULL;
    int computed_check = 0;

    // step 2: Setup
    memset(matches, 0, sizeof(matches));
    memset(partial_out, 0, sizeof(partial_out));
    input.data = text;
    input.length = text_len;
    opt = cre2_opt_new();

    // step 3: Configure
    cre2_opt_set_never_nl(opt, 1);
    flag_never_nl = cre2_opt_never_nl(opt);

    // step 4: Operate
    re = cre2_new(pattern, pattern_len, opt);
    retrieved_pattern = cre2_pattern(re);
    partial_result = cre2_partial_match(pattern, &input, partial_out, 2);
    match_result = cre2_match(re, text, text_len, 0, text_len, CRE2_UNANCHORED, matches, 3);

    // step 5: Validate
    computed_check = flag_never_nl + partial_result + match_result + matches[0].length + (int)(pattern_len);
    (void)computed_check;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}