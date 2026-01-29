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
//<ID> 108
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(foo)(bar)?";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char text[] = "prefix foobar suffix";
    const int text_len = (int)(sizeof(text) - 1);
    cre2_options_t * opt = NULL;
    cre2_string_t matches[3];
    cre2_range_t ranges[3];
    int nm = 3;
    int easy_res = 0;
    int word_boundary_flag = 0;
    cre2_encoding_t enc;

    // step 2: Setup
    memset(matches, 0, sizeof(matches));
    memset(ranges, 0, sizeof(ranges));
    matches[0].data = NULL;
    matches[0].length = 0;
    matches[1].data = NULL;
    matches[1].length = 0;
    matches[2].data = NULL;
    matches[2].length = 0;

    // step 3: Configure
    opt = cre2_opt_new();
    word_boundary_flag = cre2_opt_word_boundary(opt);
    enc = cre2_opt_encoding(opt);

    // step 4: Operate
    easy_res = cre2_easy_match(pattern, pattern_len, text, text_len, &matches[0], nm);
    cre2_strings_to_ranges(text, ranges, &matches[0], nm);

    // step 5: Validate
    (void)easy_res;
    (void)word_boundary_flag;
    (void)enc;
    (void)matches;
    (void)ranges;
    (void)text;
    (void)pattern;

    // step 6: Cleanup
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}