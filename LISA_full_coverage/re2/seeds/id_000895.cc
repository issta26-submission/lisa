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
//<ID> 895
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(foo)|(bar)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_set * set = NULL;
    int add_res = 0;
    int compile_res = 0;
    cre2_encoding_t enc = CRE2_UNKNOWN;
    int one_line_flag = 0;
    const char * pattern_from_re = NULL;
    long computed_check = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    cre2_opt_set_one_line(opt, 1);
    one_line_flag = cre2_opt_one_line(opt);

    // step 3: Core operations
    set = cre2_set_new(opt, (cre2_anchor_t)0);
    add_res = cre2_set_add_simple(set, pattern);
    compile_res = cre2_set_compile(set);
    re = cre2_new(pattern, pattern_len, opt);
    enc = cre2_opt_encoding(opt);
    pattern_from_re = cre2_pattern(re);

    // step 4: Validate / compute lightweight checksum
    computed_check = (long)pattern_len + (long)add_res + (long)compile_res + (long)enc + (long)one_line_flag + (long)strlen(pattern_from_re);
    (void)computed_check;

    // step 5: Cleanup
    cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}