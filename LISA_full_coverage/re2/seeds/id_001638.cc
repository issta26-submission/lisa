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
//<ID> 1638
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "foo(bar)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char text[] = "xx foobarbaz yy";
    const size_t text_len = (size_t)(sizeof(text) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_named_groups_iter_t * iter = NULL;
    cre2_set * set = NULL;
    int compile_res = 0;
    int match[8];
    size_t nmatched = 0;
    const char * compiled_pat = NULL;
    int errcode = 0;
    long checksum = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    cre2_opt_set_literal(opt, 1);
    cre2_opt_set_perl_classes(opt, 1);

    // step 3: Configure
    re = cre2_new(pattern, pattern_len, opt);
    iter = cre2_named_groups_iter_new(re);
    set = cre2_set_new(opt, CRE2_UNANCHORED);
    cre2_set_add_simple(set, pattern);
    compile_res = cre2_set_compile(set);

    // step 4: Operate
    compiled_pat = cre2_pattern(re);
    errcode = cre2_error_code(re);
    memset(match, -1, sizeof(match));
    nmatched = cre2_set_match(set, text, text_len, match, (size_t)(sizeof(match) / sizeof(match[0])));

    // step 5: Validate
    checksum = (long)pattern_len + (long)compile_res + (long)errcode + (long)nmatched
               + (long)match[0] + (long)match[1] + (long)(compiled_pat != NULL)
               + (long)(opt != NULL) + (long)(re != NULL) + (long)(set != NULL) + (long)(iter != NULL);
    (void)checksum;

    // step 6: Cleanup
    cre2_named_groups_iter_delete(iter);
    cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}