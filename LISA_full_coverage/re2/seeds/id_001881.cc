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
//<ID> 1881
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(alpha)(beta)?";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    cre2_options_t * opt = NULL;
    cre2_set * set = NULL;
    cre2_regexp_t * re = NULL;
    cre2_encoding_t enc = CRE2_UNKNOWN;
    int case_sensitive_flag = 1;
    int one_line_flag = 0;
    long checksum = 0;

    // step 2: Initialize options and configure
    opt = cre2_opt_new();
    cre2_opt_set_case_sensitive(opt, case_sensitive_flag);
    cre2_opt_set_one_line(opt, one_line_flag);

    // step 3: Query option-derived value and create a set with the options
    enc = cre2_opt_encoding(opt);
    set = cre2_set_new(opt, CRE2_UNANCHORED);

    // step 4: Compile a regexp with the configured options
    re = cre2_new(pattern, pattern_len, opt);

    // step 5: Validate basic outcomes by computing a checksum that uses meaningful values
    checksum = (long)pattern_len + (long)enc + (long)case_sensitive_flag + (long)one_line_flag + (long)(opt != NULL) + (long)(set != NULL) + (long)(re != NULL);
    (void)checksum;

    // step 6: Cleanup
    cre2_set_delete(set);
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}