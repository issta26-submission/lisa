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
//<ID> 368
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "ab(c+)d";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char text[] = "xxabcccdyy";
    const int text_len = (int)(sizeof(text) - 1);
    cre2_options_t *opt = NULL;
    cre2_regexp_t *re = NULL;
    cre2_string_t partial_inputs[1];
    cre2_string_t partial_outputs[2];
    cre2_string_t match_outputs[3];
    int partial_res = 0;
    int match_res = 0;
    int set_never_nl_flag = 1;
    int get_never_nl_flag = 0;
    const char *compiled_pat = NULL;
    int computed_check = 0;

    // step 2: Setup
    memset(partial_inputs, 0, sizeof(partial_inputs));
    memset(partial_outputs, 0, sizeof(partial_outputs));
    memset(match_outputs, 0, sizeof(match_outputs));
    partial_inputs[0].data = text;
    partial_inputs[0].length = text_len;
    opt = cre2_opt_new();

    // step 3: Configure
    cre2_opt_set_never_nl(opt, set_never_nl_flag);
    get_never_nl_flag = cre2_opt_never_nl(opt);

    // step 4: Operate
    partial_res = cre2_partial_match(pattern, partial_inputs, partial_outputs, 1);
    re = cre2_new(pattern, pattern_len, opt);
    match_res = cre2_match(re, text, text_len, 0, text_len, CRE2_UNANCHORED, match_outputs, 3);
    compiled_pat = cre2_pattern(re);

    // step 5: Validate
    computed_check = partial_res + match_res + get_never_nl_flag + partial_outputs[0].length + match_outputs[0].length + (int)(compiled_pat ? compiled_pat[0] : 0);
    (void)computed_check;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}