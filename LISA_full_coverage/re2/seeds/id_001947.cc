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
//<ID> 1947
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(hello)(world)?";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    const char text[] = "helloworld hello";
    const int text_len = (int)(sizeof(text) - 1);
    cre2_string_t matches[4];
    int nmatch = 4;
    int match_ret = 0;
    long checksum = 0;

    // step 2: Setup (initialize options and configure)
    opt = cre2_opt_new();
    cre2_opt_set_case_sensitive(opt, 1);
    cre2_opt_set_one_line(opt, 1);

    // step 3: Core operations (compile and perform match)
    re = cre2_new(pattern, pattern_len, opt);
    memset(matches, 0, sizeof(matches));
    match_ret = cre2_match(re, text, text_len, 0, text_len, CRE2_UNANCHORED, matches, nmatch);

    // step 4: Validate (combine results into checksum to ensure values are used)
    checksum = (long)pattern_len + (long)text_len + (long)match_ret + (long)nmatch + (long)(opt != NULL) + (long)(re != NULL) + (long)matches[0].length;
    (void)checksum;

    // step 5: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}