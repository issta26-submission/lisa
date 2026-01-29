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
//<ID> 1122
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
    cre2_regexp_t * re = NULL;
    cre2_string_t matches[3];
    const int nmatch = (int)(sizeof(matches) / sizeof(matches[0]));
    int posix_flag = 0;
    int match_ret = 0;
    const char * compiled_pattern = NULL;
    long computed_check = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    cre2_opt_set_posix_syntax(opt, 1);
    cre2_opt_set_case_sensitive(opt, 1);
    posix_flag = cre2_opt_posix_syntax(opt);

    // step 3: Core operations
    memset(matches, 0, sizeof(matches));
    re = cre2_new(pattern, pattern_len, opt);
    compiled_pattern = cre2_pattern(re);
    match_ret = cre2_match(re, text_buf, text_len, 0, text_len, CRE2_UNANCHORED, matches, nmatch);

    // step 4: Validate / compute lightweight checksum
    computed_check = (long)posix_flag + (long)match_ret + (long)matches[0].length + (long)pattern_len + (long)text_len + (long)compiled_pattern[0];
    (void)computed_check;

    // step 5: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}