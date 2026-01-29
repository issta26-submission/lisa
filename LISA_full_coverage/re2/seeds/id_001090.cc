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
//<ID> 1090
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "([A-Za-z]+)([0-9]*)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    char text_buf[] = "alpha123 beta gamma456";
    const int text_len = (int)(sizeof(text_buf) - 1);
    cre2_options_t * opt = NULL;
    cre2_set * set = NULL;
    cre2_regexp_t * re = NULL;
    cre2_string_t input_text;
    cre2_string_t matches[4];
    const int nmatch = (int)(sizeof(matches) / sizeof(matches[0]));
    int compile_ret = 0;
    int partial_ret = 0;
    int num_groups = 0;
    long computed_check = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    cre2_opt_set_case_sensitive(opt, 1);
    cre2_opt_set_longest_match(opt, 1);
    set = cre2_set_new(opt, CRE2_UNANCHORED);
    cre2_set_add_simple(set, pattern);

    // step 3: Compile the set
    compile_ret = cre2_set_compile(set);

    // step 4: Core operations (create regexp, inspect, and partial match)
    re = cre2_new(pattern, pattern_len, opt);
    num_groups = cre2_num_capturing_groups(re);
    input_text.data = text_buf;
    input_text.length = text_len;
    memset(matches, 0, sizeof(matches));
    partial_ret = cre2_partial_match(pattern, &input_text, matches, nmatch);

    // step 5: Validate / compute lightweight checksum
    computed_check = (long)compile_ret + (long)partial_ret + (long)num_groups + (long)pattern_len + (long)matches[0].length;
    (void)computed_check;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}