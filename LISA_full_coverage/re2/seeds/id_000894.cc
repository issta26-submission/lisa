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
//<ID> 894
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(test)(pattern)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char set_pattern[] = "simple";
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    struct cre2_set * set = NULL;
    cre2_encoding_t enc = CRE2_UNKNOWN;
    int one_line_flag_set = 1;
    int one_line_flag_read = 0;
    int add_res = 0;
    const char * pattern_from_re = NULL;
    long computed_check = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    cre2_opt_set_one_line(opt, one_line_flag_set);
    one_line_flag_read = cre2_opt_one_line(opt);
    enc = cre2_opt_encoding(opt);

    // step 3: Core operations
    re = cre2_new(pattern, pattern_len, opt);
    set = cre2_set_new(opt, (cre2_anchor_t)0);
    add_res = cre2_set_add_simple(set, set_pattern);
    pattern_from_re = cre2_pattern(re);

    // step 4: Validate / compute lightweight checksum
    computed_check = (long)pattern_len + (long)one_line_flag_set + (long)one_line_flag_read + (long)enc + (long)add_res + (long)strlen(pattern_from_re);
    (void)computed_check;

    // step 5: Cleanup
    cre2_set_delete(set);
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}