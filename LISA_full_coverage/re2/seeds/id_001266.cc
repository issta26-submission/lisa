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
//<ID> 1266
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(foo)([0-9]+)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    char text_buf[] = "foo123 and then foo456";
    const int text_len = (int)(sizeof(text_buf) - 1);
    cre2_options_t * opt = NULL;
    cre2_set * set = NULL;
    cre2_regexp_t * re = NULL;
    cre2_string_t input;
    cre2_string_t match;
    const char * pattern_from_re = NULL;
    int partial_ret = 0;
    int set_add_ret = 0;
    long computed_check = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    set = cre2_set_new(opt, (cre2_anchor_t)0);
    memset(&input, 0, sizeof(input));
    memset(&match, 0, sizeof(match));
    input.data = text_buf;
    input.length = text_len;
    match.data = NULL;
    match.length = 0;

    // step 3: Configure
    cre2_opt_set_posix_syntax(opt, 1);
    set_add_ret = cre2_set_add_simple(set, pattern);

    // step 4: Core operations
    re = cre2_new(pattern, pattern_len, opt);
    pattern_from_re = cre2_pattern(re);
    partial_ret = cre2_partial_match(pattern_from_re, &input, &match, 3);

    // step 5: Validate / compute lightweight checksum
    computed_check = (long)pattern_len + (long)text_len + (long)partial_ret + (long)set_add_ret
                     + (long)match.length + (long)(re != NULL) + (long)(set != NULL) + (long)(opt != NULL)
                     + (long)(pattern_from_re != NULL) + (long)input.length;
    (void)computed_check;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}