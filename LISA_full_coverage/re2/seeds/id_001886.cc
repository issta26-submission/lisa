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
//<ID> 1886
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
    int add_res1 = 0;
    int add_res2 = 0;
    const char * recovered_pattern = NULL;
    int errcode = 0;
    long checksum = 0;
    char scratch[64];
    memset(scratch, 0, sizeof(scratch));

    // step 2: Setup
    opt = cre2_opt_new();
    cre2_opt_set_case_sensitive(opt, 0);
    cre2_opt_set_one_line(opt, 1);
    enc = cre2_opt_encoding(opt);
    set = cre2_set_new(opt, CRE2_UNANCHORED);
    add_res1 = cre2_set_add_simple(set, "alpha");
    add_res2 = cre2_set_add_simple(set, "beta");
    cre2_set_compile(set);
    re = cre2_new(pattern, pattern_len, opt);

    // step 3: Core operations
    recovered_pattern = cre2_pattern(re);
    errcode = cre2_error_code(re);

    // step 4: Validate
    checksum = (long)pattern_len + (long)enc + (long)add_res1 + (long)add_res2 + (long)(recovered_pattern != NULL) + (long)errcode + (long)(opt != NULL) + (long)(set != NULL) + (long)(re != NULL);
    (void)checksum;
    (void)scratch;

    // step 5: Cleanup
    cre2_set_delete(set);
    cre2_delete(re);
    cre2_opt_delete(opt);

    // step 6: API sequence test completed successfully
    return 66;
}