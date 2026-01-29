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
//<ID> 1953
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(a)(b)?";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char text[] = "ab ab";
    const int text_len = (int)(sizeof(text) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_encoding_t enc = CRE2_UNKNOWN;
    cre2_string_t matches[3] = { {NULL, 0}, {NULL, 0}, {NULL, 0} };
    cre2_named_groups_iter_t * iter = NULL;
    const char * errstr = NULL;
    int log_errors_flag = 0;
    int easy_ret = 0;
    long checksum = 0;

    // step 2: Setup (initialize and configure options)
    opt = cre2_opt_new();
    cre2_opt_set_case_sensitive(opt, 1);
    cre2_opt_set_one_line(opt, 1);
    cre2_opt_set_log_errors(opt, 1);
    log_errors_flag = cre2_opt_log_errors(opt);
    enc = cre2_opt_encoding(opt);

    // step 3: Core operations (compile and query)
    re = cre2_new(pattern, pattern_len, opt);
    errstr = cre2_error_string(re);
    iter = cre2_named_groups_iter_new(re);
    easy_ret = cre2_easy_match(pattern, pattern_len, text, text_len, matches, 3);

    // step 4: Validate (produce a checksum that depends on API outputs)
    checksum = 0;
    checksum += (long)pattern_len;
    checksum += (long)text_len;
    checksum += (long)enc;
    checksum += (long)log_errors_flag;
    checksum += (long)easy_ret;
    checksum += (long)matches[0].length;
    checksum += (long)matches[1].length;
    checksum += (long)matches[2].length;
    checksum += (long)(errstr != NULL);
    checksum += (long)(iter != NULL);
    checksum += (long)(opt != NULL);
    checksum += (long)(re != NULL);
    (void)checksum;

    // step 5: Cleanup
    cre2_named_groups_iter_delete(iter);
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}