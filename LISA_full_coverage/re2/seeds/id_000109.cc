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
//<ID> 109
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(test)(ing)?";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char input_text[] = "this is a testing string for cre2";
    const int input_text_len = (int)(sizeof(input_text) - 1);
    cre2_options_t * opt = NULL;
    cre2_encoding_t enc = CRE2_UNKNOWN;
    int word_boundary_flag = 0;
    cre2_string_t matches[4];
    cre2_range_t ranges[4];
    int nm = 4;
    int easy_res = 0;

    // step 2: Setup
    memset(&matches, 0, sizeof(matches));
    memset(&ranges, 0, sizeof(ranges));
    opt = cre2_opt_new();

    // step 3: Configure / Query options
    word_boundary_flag = cre2_opt_word_boundary(opt);
    enc = cre2_opt_encoding(opt);

    // step 4: Operate - perform a simple match and convert found strings to ranges
    easy_res = cre2_easy_match(pattern, pattern_len, input_text, input_text_len, &matches[0], nm);
    cre2_strings_to_ranges(input_text, ranges, &matches[0], nm);

    // step 5: Validate (use results to satisfy data-flow; no branching)
    (void)easy_res;
    (void)word_boundary_flag;
    (void)enc;
    (void)matches;
    (void)ranges;
    (void)input_text;
    (void)input_text_len;

    // step 6: Cleanup
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}