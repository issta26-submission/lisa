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
//<ID> 1624
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(a|b)\\w+";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_set * set = NULL;
    const char * compiled_pat = NULL;
    int errcode = 0;
    int add_result = 0;
    int compile_result = 0;
    char scratch[16];
    long checksum = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    cre2_opt_set_word_boundary(opt, 1);
    set = cre2_set_new(opt, CRE2_UNANCHORED);

    // step 3: Configure
    add_result = cre2_set_add_simple(set, pattern);
    compile_result = cre2_set_compile(set);
    re = cre2_new(pattern, pattern_len, opt);

    // step 4: Operate
    compiled_pat = cre2_pattern(re);
    errcode = cre2_error_code(re);
    memset(scratch, 0, sizeof(scratch));

    // step 5: Validate
    checksum = (long)pattern_len + (long)(compiled_pat != NULL) + (long)errcode + (long)add_result + (long)compile_result
               + (long)(opt != NULL) + (long)(re != NULL) + (long)(set != NULL) + (long)scratch[0];
    (void)checksum;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}