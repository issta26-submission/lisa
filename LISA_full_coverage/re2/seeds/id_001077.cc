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
//<ID> 1077
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(a)(b)(c)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    char text_buf[] = "abc xyz abc";
    const int text_len = (int)(sizeof(text_buf) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_named_groups_iter_t * iter = NULL;
    cre2_string_t match[4];
    memset(match, 0, sizeof(match));
    const int nmatch = 4;
    int match_result = 0;
    int enc_val = 0;
    int posix_flag = 0;
    long computed_check = 0;

    // step 2: Setup (initialize options and query encoding)
    opt = cre2_opt_new();
    cre2_opt_set_case_sensitive(opt, 1);
    cre2_opt_set_log_errors(opt, 1);
    enc_val = (int)cre2_opt_encoding(opt);

    // step 3: Core operations (compile the regular expression and create iterator)
    re = cre2_new(pattern, pattern_len, opt);
    iter = cre2_named_groups_iter_new(re);

    // step 4: Operate (perform a match)
    match_result = cre2_match(re, text_buf, text_len, 0, text_len, CRE2_UNANCHORED, match, nmatch);
    posix_flag = cre2_opt_posix_syntax(opt);

    // step 5: Validate / compute lightweight checksum
    computed_check = (long)enc_val + (long)posix_flag + (long)match_result + (long)pattern_len + (long)text_len + (long)match[0].length;
    (void)computed_check;

    // step 6: Cleanup
    cre2_named_groups_iter_delete(iter);
    cre2_delete(re);
    cre2_opt_delete(opt);

    return 66;
    // API sequence test completed successfully
}