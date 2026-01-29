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
//<ID> 32
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "abc(\\d+)?";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char text[] = "abc123xyz";
    const int text_len = (int)(sizeof(text) - 1);
    cre2_options_t * opt = cre2_opt_new();
    cre2_string_t text_str;
    /* Allocate arrays sized to the maximum possible nmatch computed below (<=3).
       This prevents stack smashing that can occur if the library writes up to nmatch
       entries but we only provided a single cre2_string_t. */
    cre2_string_t match_partial_arr[3];
    cre2_string_t match_easy_arr[3];
    int literal_flag = 0;
    int case_flag = 0;
    int nmatch = 1;
    int partial_res = 0;
    int easy_res = 0;

    // step 2: Setup
    memset(&text_str, 0, sizeof(text_str));
    memset(match_partial_arr, 0, sizeof(match_partial_arr));
    memset(match_easy_arr, 0, sizeof(match_easy_arr));
    text_str.data = text;
    text_str.length = text_len;

    // step 3: Configure
    if (opt) {
        cre2_opt_set_literal(opt, 1);
        cre2_opt_set_case_sensitive(opt, 1);
        literal_flag = cre2_opt_literal(opt);
        case_flag = cre2_opt_case_sensitive(opt);
    }
    nmatch = literal_flag + case_flag + 1;

    /* Ensure we do not pass an nmatch larger than the arrays we've allocated.
       According to the computation above nmatch <= 3, but clamp defensively. */
    {
        const int max_slots = (int)(sizeof(match_partial_arr) / sizeof(match_partial_arr[0]));
        if (nmatch > max_slots) nmatch = max_slots;
        if (nmatch < 1) nmatch = 1;
    }

    // step 4: Operate
    partial_res = cre2_partial_match(pattern, &text_str, match_partial_arr, nmatch);
    easy_res = cre2_easy_match(pattern, pattern_len, text, text_len, match_easy_arr, nmatch);

    // step 5: Validate & Cleanup
    if (opt) cre2_opt_delete(opt);

    /* Silence unused-variable warnings while keeping logic and return value unchanged. */
    (void)partial_res;
    (void)easy_res;
    (void)literal_flag;
    (void)case_flag;
    (void)nmatch;
    (void)text_str;
    for (int i = 0; i < (int)(sizeof(match_partial_arr) / sizeof(match_partial_arr[0])); ++i) {
        (void)match_partial_arr[i];
        (void)match_easy_arr[i];
    }

    // API sequence test completed successfully
    return 66;
}