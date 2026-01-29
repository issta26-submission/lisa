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
//<ID> 1954
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(?P<g>foo)bar";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_named_groups_iter_t * iter = NULL;
    cre2_encoding_t enc = CRE2_UNKNOWN;
    int log_flag = 0;
    const char * errstr = NULL;
    const char text[] = "foobar xyz foobar";
    const int text_len = (int)(sizeof(text) - 1);
    cre2_string_t matches[2] = { { NULL, 0 }, { NULL, 0 } };
    int match_ret = 0;
    long checksum = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    cre2_opt_set_log_errors(opt, 1);
    enc = cre2_opt_encoding(opt);
    log_flag = cre2_opt_log_errors(opt);

    // step 3: Core operations (compile and inspect)
    re = cre2_new(pattern, pattern_len, opt);
    errstr = cre2_error_string(re);
    iter = cre2_named_groups_iter_new(re);

    // step 4: Operate (perform an easy match using the same pattern)
    match_ret = cre2_easy_match(pattern, pattern_len, text, text_len, matches, 2);

    // step 5: Validate
    checksum = (long)pattern_len + (long)enc + (long)log_flag + (long)match_ret + (long)matches[0].length + (long)matches[1].length + (long)(opt != NULL) + (long)(re != NULL) + (long)(iter != NULL) + (long)(errstr != NULL);
    (void)checksum;

    // step 6: Cleanup
    cre2_named_groups_iter_delete(iter);
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}