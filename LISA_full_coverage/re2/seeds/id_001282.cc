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
//<ID> 1282
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "user([0-9]+)@example\\.com";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    char text_buf[] = "Contact: user123@example.com for info";
    const int text_len = (int)(sizeof(text_buf) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_set * set = NULL;
    cre2_string_t matches[4];
    int set_matches[4];
    int set_add_ret = 0;
    int set_compile_ret = 0;
    size_t set_match_count = 0;
    int cre2_match_ret = 0;
    const char * re_pat = NULL;
    long computed_check = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    set = cre2_set_new(opt, CRE2_UNANCHORED);
    memset(matches, 0, sizeof(matches));
    memset(set_matches, 0, sizeof(set_matches));
    matches[0].data = NULL;
    matches[0].length = 0;

    // step 3: Configure
    cre2_opt_set_one_line(opt, 1);
    set_add_ret = cre2_set_add_simple(set, pattern);
    set_compile_ret = cre2_set_compile(set);

    // step 4: Core operations
    re = cre2_new(pattern, pattern_len, opt);
    cre2_match_ret = cre2_match(re, text_buf, text_len, 0, text_len, CRE2_UNANCHORED, matches, 3);
    set_match_count = cre2_set_match(set, text_buf, (size_t)text_len, set_matches, 4);
    re_pat = cre2_pattern(re);

    // step 5: Validate / compute lightweight checksum
    computed_check = (long)pattern_len + (long)text_len + (long)set_add_ret + (long)set_compile_ret
                     + (long)cre2_match_ret + (long)set_match_count + (long)(matches[0].length)
                     + (long)(re != NULL) + (long)(re_pat != NULL) + (long)(opt != NULL) + (long)(set != NULL);
    (void)computed_check;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}