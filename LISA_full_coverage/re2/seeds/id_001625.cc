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
//<ID> 1625
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "\\bfoo(bar)?\\b";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char text[] = "xx foobar yy foo zz";
    const size_t text_len = (size_t)(sizeof(text) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_set * set = NULL;
    const char * compiled_pat = NULL;
    int err_code = 0;
    int match_indices[4];
    size_t matched = 0;
    int add_res = 0;
    int compile_res = 0;
    long checksum = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    cre2_opt_set_word_boundary(opt, 1);

    // step 3: Configure
    re = cre2_new(pattern, pattern_len, opt);
    compiled_pat = cre2_pattern(re);
    err_code = cre2_error_code(re);
    set = cre2_set_new(opt, CRE2_ANCHOR_START);
    add_res = cre2_set_add_simple(set, pattern);
    compile_res = cre2_set_compile(set);

    // step 4: Operate
    memset(match_indices, 0xff, sizeof(match_indices));
    matched = cre2_set_match(set, text, text_len, match_indices, 4);

    // step 5: Validate
    checksum = (long)pattern_len + (long)err_code + (long)compile_res + (long)add_res + (long)matched
               + (long)match_indices[0] + (long)match_indices[1] + (long)(compiled_pat != NULL);
    (void)checksum;

    // step 6: Cleanup
    cre2_set_delete(set);
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}