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
//<ID> 1878
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "abc";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char set_pattern[] = "ab";
    cre2_options_t * opt = NULL;
    cre2_set * set = NULL;
    cre2_regexp_t * re = NULL;
    int add_res = 0;
    cre2_encoding_t enc = CRE2_UNKNOWN;
    const char * errstr = NULL;
    long checksum = 0;

    // step 2: Initialize
    opt = cre2_opt_new();

    // step 3: Configure
    enc = cre2_opt_encoding(opt);
    set = cre2_set_new(opt, CRE2_UNANCHORED);
    add_res = cre2_set_add_simple(set, set_pattern);
    cre2_set_compile(set);

    // step 4: Operate
    re = cre2_new(pattern, pattern_len, opt);
    errstr = cre2_error_string(re);

    // step 5: Validate
    checksum = (long)pattern_len + (long)add_res + (long)enc + (long)(opt != NULL) + (long)(set != NULL) + (long)(re != NULL) + (long)(errstr != NULL);
    (void)checksum;

    // step 6: Cleanup
    cre2_set_delete(set);
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}