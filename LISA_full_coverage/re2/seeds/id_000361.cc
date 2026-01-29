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
//<ID> 361
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(h[ae]llo)\\s+(world)";
    int pattern_len = (int)(sizeof(pattern) - 1);
    const char text[] = "hello   world";
    int text_len = (int)(sizeof(text) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_string_t matches[4];
    cre2_string_t partial_matches[4];
    cre2_string_t text_str;
    int flag_never_nl = 0;
    int match_count = 0;
    int partial_result = 0;
    const char * compiled_pattern = NULL;
    int computed_check = 0;

    // step 2: Setup
    memset(matches, 0, sizeof(matches));
    memset(partial_matches, 0, sizeof(partial_matches));
    opt = cre2_opt_new();
    cre2_opt_set_never_nl(opt, 1);
    flag_never_nl = cre2_opt_never_nl(opt);
    re = cre2_new(pattern, pattern_len, opt);
    text_str.data = text;
    text_str.length = text_len;

    // step 3: Operate (compiled match)
    match_count = cre2_match(re, text, text_len, 0, text_len, CRE2_UNANCHORED, matches, 3);

    // step 4: Operate (partial match using pattern string API)
    partial_result = cre2_partial_match(pattern, &text_str, partial_matches, 3);

    // step 5: Validate
    compiled_pattern = cre2_pattern(re);
    computed_check = (int)(compiled_pattern ? compiled_pattern[0] : 0);
    computed_check += flag_never_nl + match_count + partial_result + matches[0].length + partial_matches[0].length + text_str.length;
    (void)computed_check;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}