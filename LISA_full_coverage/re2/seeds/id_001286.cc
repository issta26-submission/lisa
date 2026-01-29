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
//<ID> 1286
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "user([0-9]+):([a-z]+)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    char text_buf[] = "log: user123:john action";
    const int text_len = (int)(sizeof(text_buf) - 1);
    cre2_options_t * opt = NULL;
    cre2_set * set = NULL;
    cre2_regexp_t * re = NULL;
    cre2_string_t matches[3];
    int set_match_indices[4];
    int set_add_ret = 0;
    int set_compile_ret = 0;
    size_t set_matches = 0;
    int match_ret = 0;
    const char * re_pat = NULL;
    long computed_check = 0;

    // step 2: Setup
    memset(&matches, 0, sizeof(matches));
    memset(&set_match_indices, -1, sizeof(set_match_indices));
    opt = cre2_opt_new();
    set = cre2_set_new(opt, CRE2_UNANCHORED);

    // step 3: Configure
    set_add_ret = cre2_set_add_simple(set, pattern);
    set_compile_ret = cre2_set_compile(set);
    re = cre2_new(pattern, pattern_len, opt);

    // step 4: Core operations
    set_matches = cre2_set_match(set, text_buf, (size_t)text_len, set_match_indices, (size_t)(sizeof(set_match_indices)/sizeof(set_match_indices[0])));
    match_ret = cre2_match(re, text_buf, text_len, 0, text_len, CRE2_UNANCHORED, matches, 3);
    re_pat = cre2_pattern(re);

    // step 5: Validate / compute lightweight checksum
    computed_check = (long)pattern_len + (long)text_len + (long)set_add_ret + (long)set_compile_ret
                     + (long)set_matches + (long)match_ret + (long)(re != NULL) + (long)(opt != NULL)
                     + (long)(set != NULL) + (long)(re_pat != NULL)
                     + (long)matches[0].length + (long)matches[1].length + (long)matches[2].length;
    (void)computed_check;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}