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
//<ID> 1287
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "item([0-9]+)-X";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    char text_buf[] = "prefix item123-X suffix";
    const int text_len = (int)(sizeof(text_buf) - 1);
    cre2_options_t * opt = NULL;
    cre2_set * set = NULL;
    cre2_regexp_t * re = NULL;
    cre2_string_t match;
    int set_match_indices[4];
    size_t set_matches = 0;
    int cre2_match_ret = 0;
    const char * re_pat = NULL;
    long computed_check = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    set = cre2_set_new(opt, CRE2_UNANCHORED);
    memset(&match, 0, sizeof(match));
    memset(set_match_indices, 0xff, sizeof(set_match_indices)); // initialize to -1

    // step 3: Configure
    cre2_set_add_simple(set, pattern);
    cre2_set_compile(set);

    // step 4: Core operations
    re = cre2_new(pattern, pattern_len, opt);
    re_pat = cre2_pattern(re);
    cre2_match_ret = cre2_match(re, text_buf, text_len, 0, text_len, CRE2_UNANCHORED, &match, 1);
    set_matches = cre2_set_match(set, text_buf, (size_t)text_len, set_match_indices, sizeof(set_match_indices) / sizeof(set_match_indices[0]));

    // step 5: Validate / compute lightweight checksum
    computed_check = (long)pattern_len + (long)text_len + (long)cre2_match_ret + (long)set_matches
                     + (long)(re != NULL) + (long)(re_pat != NULL) + (long)(match.length)
                     + (long)set_match_indices[0];
    (void)computed_check;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}