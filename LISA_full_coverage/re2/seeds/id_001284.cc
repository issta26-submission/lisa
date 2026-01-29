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
//<ID> 1284
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "item([0-9]+)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    char text_buf[] = "prefix item123 suffix";
    const int text_len = (int)(sizeof(text_buf) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_set * set = NULL;
    cre2_string_t input;
    cre2_string_t match;
    int set_add_ret = 0;
    int set_compile_ret = 0;
    size_t set_match_ret = 0;
    int match_indices[8];
    int cre2_match_ret = 0;
    const char * re_pat = NULL;
    long computed_check = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    set = cre2_set_new(opt, CRE2_UNANCHORED);
    memset(&input, 0, sizeof(input));
    memset(&match, 0, sizeof(match));
    input.data = text_buf;
    input.length = text_len;
    match.data = NULL;
    match.length = 0;
    memset(match_indices, 0xFF, sizeof(match_indices));

    // step 3: Configure
    cre2_opt_set_case_sensitive(opt, 1);
    set_add_ret = cre2_set_add_simple(set, pattern);
    set_compile_ret = cre2_set_compile(set);

    // step 4: Core operations
    re = cre2_new(pattern, pattern_len, opt);
    cre2_match_ret = cre2_match(re, text_buf, text_len, 0, text_len, CRE2_UNANCHORED, &match, 1);
    set_match_ret = cre2_set_match(set, text_buf, (size_t)text_len, match_indices, (size_t)(sizeof(match_indices) / sizeof(match_indices[0])));
    re_pat = cre2_pattern(re);

    // step 5: Validate / compute lightweight checksum
    computed_check = (long)pattern_len + (long)text_len + (long)set_add_ret + (long)set_compile_ret
                     + (long)cre2_match_ret + (long)set_match_ret + (long)(re != NULL)
                     + (long)(re_pat != NULL) + (long)match.length + (long)(opt != NULL) + (long)(set != NULL);
    (void)computed_check;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}