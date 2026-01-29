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
//<ID> 1957
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(foo)(bar)?";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char text[] = "foobar foobar";
    const int text_len = (int)(sizeof(text) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_named_groups_iter_t * iter = NULL;
    cre2_encoding_t enc = CRE2_UNKNOWN;
    int log_errors_flag = 0;
    cre2_string_t matches[3];
    int match_ret = 0;
    const char * errstr = NULL;
    long checksum = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    cre2_opt_set_log_errors(opt, 1);
    enc = cre2_opt_encoding(opt);
    log_errors_flag = cre2_opt_log_errors(opt);

    // step 3: Operate (easy match)
    memset(matches, 0, sizeof(matches));
    match_ret = cre2_easy_match(pattern, pattern_len, text, text_len, matches, 3);

    // step 4: Core operations (compile and inspect)
    re = cre2_new(pattern, pattern_len, opt);
    errstr = cre2_error_string(re);
    iter = cre2_named_groups_iter_new(re);

    // step 5: Validate (compute a checksum from results)
    checksum = (long)pattern_len + (long)text_len + (long)match_ret + (long)enc + (long)log_errors_flag + (long)matches[0].length + (long)matches[1].length + (long)matches[2].length + (long)(errstr != NULL) + (long)(re != NULL) + (long)(iter != NULL);
    (void)checksum;

    // step 6: Cleanup
    cre2_named_groups_iter_delete(iter);
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}