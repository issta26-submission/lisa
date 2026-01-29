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
//<ID> 1298
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "([A-Za-z]+)-(\\d+)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    char text_buf[] = "Token-1234 trailing";
    const int text_len = (int)(sizeof(text_buf) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_string_t match;
    int match_ret = 0;
    int re_match_ret = 0;
    int prog_size = 0;
    int perl_flag = 0;
    int64_t max_mem_val = 0;
    long computed_check = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    memset(&match, 0, sizeof(match));
    match.data = NULL;
    match.length = 0;

    // step 3: Configure
    cre2_opt_set_literal(opt, 1);
    cre2_opt_set_perl_classes(opt, 1);
    cre2_opt_set_max_mem(opt, (int64_t) (4 * 1024 * 1024));
    perl_flag = cre2_opt_perl_classes(opt);
    max_mem_val = cre2_opt_max_mem(opt);

    // step 4: Core operations
    re = cre2_new(pattern, pattern_len, opt);
    prog_size = cre2_program_size(re);
    match_ret = cre2_easy_match(pattern, pattern_len, text_buf, text_len, &match, 2);
    re_match_ret = cre2_match(re, text_buf, text_len, 0, text_len, CRE2_UNANCHORED, &match, 2);

    // step 5: Validate / compute lightweight checksum
    computed_check = (long)pattern_len + (long)text_len + (long)prog_size + (long)match_ret
                     + (long)re_match_ret + (long)perl_flag + (long)max_mem_val
                     + (long)(match.length) + (long)(re != NULL) + (long)(opt != NULL);
    (void)computed_check;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}