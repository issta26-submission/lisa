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
//<ID> 101
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(foo)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char text[] = "foo and foo again";
    const int text_len = (int)(sizeof(text) - 1);
    cre2_options_t * opt = NULL;
    cre2_string_t matches[3];
    cre2_range_t ranges[3];
    int nm = 2; /* overall match + one capturing group */
    int easy_res = 0;
    cre2_encoding_t enc = CRE2_UNKNOWN;
    int word_boundary_flag = 0;

    // step 2: Setup
    memset(matches, 0, sizeof(matches));
    memset(ranges, 0, sizeof(ranges));
    opt = cre2_opt_new();

    // step 3: Configure / Inspect options
    word_boundary_flag = cre2_opt_word_boundary(opt);
    enc = cre2_opt_encoding(opt);

    // step 4: Operate - perform a simple match and convert matched strings to ranges
    easy_res = cre2_easy_match(pattern, pattern_len, text, text_len, &matches[0], nm);
    cre2_strings_to_ranges(text, &ranges[0], &matches[0], nm);

    // step 5: Validate (use results meaningfully by keeping them referenced)
    (void)word_boundary_flag;
    (void)enc;
    (void)easy_res;
    (void)matches;
    (void)ranges;

    // step 6: Cleanup
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}