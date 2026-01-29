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
//<ID> 1872
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
    char scratch_buf[64];
    memset(scratch_buf, 0, sizeof(scratch_buf));
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_set * set = NULL;
    const char * errstr = NULL;
    cre2_encoding_t enc = CRE2_UNKNOWN;
    int add_res1 = 0;
    int add_res2 = 0;
    long checksum = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    cre2_opt_set_encoding(opt, CRE2_UTF8);
    enc = cre2_opt_encoding(opt);
    set = cre2_set_new(opt, CRE2_UNANCHORED);
    add_res1 = cre2_set_add_simple(set, "foo");
    add_res2 = cre2_set_add_simple(set, "bar");
    cre2_set_compile(set);

    // step 3: Core operations
    re = cre2_new(pattern, pattern_len, opt);
    errstr = cre2_error_string(re);

    // step 4: Validate
    checksum = (long)pattern_len + (long)enc + (long)add_res1 + (long)add_res2 + (long)(errstr != NULL) + (long)(opt != NULL) + (long)(set != NULL) + (long)(re != NULL) + (long)scratch_buf[0];
    (void)checksum;

    // step 5: Additional sanity op
    memset(scratch_buf, 0xAA, 1);

    // step 6: Cleanup
    cre2_set_delete(set);
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}