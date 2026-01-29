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
//<ID> 106
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
    cre2_string_t matches[2];
    cre2_range_t ranges[2];
    int nmatch = 2;
    int easy_res = 0;
    int word_boundary_flag = 0;
    cre2_encoding_t enc = CRE2_UNKNOWN;
    int first_match_len = 0;

    // step 2: Setup
    memset(matches, 0, sizeof(matches));
    memset(ranges, 0, sizeof(ranges));
    opt = cre2_opt_new();

    // step 3: Configure
    word_boundary_flag = cre2_opt_word_boundary(opt);
    enc = cre2_opt_encoding(opt);

    // step 4: Operate
    easy_res = cre2_easy_match(pattern, pattern_len, text, text_len, &matches[0], nmatch);
    cre2_strings_to_ranges(text, ranges, &matches[0], nmatch);

    // step 5: Validate
    first_match_len = (int)(ranges[0].past - ranges[0].start);
    (void)easy_res;
    (void)word_boundary_flag;
    (void)enc;
    (void)first_match_len;
    (void)matches;
    (void)ranges;

    // step 6: Cleanup
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}