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
//<ID> 1942
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
    const char text[] = "helloworld hello";
    const int text_len = (int)(sizeof(text) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_string_t match0;
    cre2_string_t match1;
    cre2_string_t match2;
    cre2_string_t matches[3];
    const int nmatch = 3;
    int match_ret = -1;
    int startpos = 0;
    int endpos = text_len;
    cre2_anchor_t anchor = CRE2_UNANCHORED;
    int one_line_flag = 0;
    long checksum = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    cre2_opt_set_case_sensitive(opt, 1);
    cre2_opt_set_one_line(opt, 1);
    one_line_flag = cre2_opt_one_line(opt);

    // step 3: Core operations
    re = cre2_new(pattern, pattern_len, opt);
    match0.data = NULL;
    match0.length = 0;
    match1.data = NULL;
    match1.length = 0;
    match2.data = NULL;
    match2.length = 0;
    matches[0] = match0;
    matches[1] = match1;
    matches[2] = match2;
    match_ret = cre2_match(re, text, text_len, startpos, endpos, anchor, matches, nmatch);

    // step 4: Validate
    checksum = (long)pattern_len + (long)text_len + (long)match_ret + (long)matches[0].length + (long)matches[1].length + (long)one_line_flag + (long)(opt != NULL) + (long)(re != NULL);
    (void)checksum;

    // step 5: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // step 6: Finalize
    // API sequence test completed successfully
    return 66;
}