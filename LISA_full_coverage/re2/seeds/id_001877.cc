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
//<ID> 1877
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern_main[] = "(foo)(bar)?";
    const int pattern_main_len = (int)(sizeof(pattern_main) - 1);
    const char set_pat1[] = "foo";
    const char set_pat2[] = "bar";
    cre2_options_t * opt = NULL;
    cre2_set * set = NULL;
    cre2_regexp_t * re = NULL;
    const char * errstr = NULL;
    cre2_encoding_t enc = CRE2_UNKNOWN;
    int add_res1 = 0;
    int add_res2 = 0;
    long checksum = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    enc = cre2_opt_encoding(opt);
    set = cre2_set_new(opt, CRE2_UNANCHORED);

    // step 3: Operate
    add_res1 = cre2_set_add_simple(set, set_pat1);
    add_res2 = cre2_set_add_simple(set, set_pat2);
    cre2_set_compile(set);
    re = cre2_new(pattern_main, pattern_main_len, opt);
    errstr = cre2_error_string(re);

    // step 4: Validate
    checksum = (long)pattern_main_len + (long)add_res1 + (long)add_res2 + (long)(opt != NULL) + (long)(set != NULL) + (long)(re != NULL) + (long)(errstr != NULL) + (long)enc;
    (void)checksum;

    // step 5: Cleanup
    cre2_set_delete(set);
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}