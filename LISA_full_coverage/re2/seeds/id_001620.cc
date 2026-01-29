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
//<ID> 1620
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(foo|bar)baz";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char add_pattern[] = "foo(bar)";
    const char text[] = "foobarbaz";
    const int text_len = (int)(sizeof(text) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_set * set = NULL;
    const char * compiled_pat = NULL;
    int errcode = 0;
    int add_ret = 0;
    int compile_ret = 0;
    size_t match_count = 0;
    int match[4];
    long checksum = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    cre2_opt_set_word_boundary(opt, 1);

    // step 3: Configure
    re = cre2_new(pattern, pattern_len, opt);
    compiled_pat = cre2_pattern(re);

    // step 4: Operate
    set = cre2_set_new(opt, CRE2_UNANCHORED);
    add_ret = cre2_set_add_simple(set, add_pattern);
    compile_ret = cre2_set_compile(set);
    memset(match, 0, sizeof(match));
    match_count = cre2_set_match(set, text, (size_t)text_len, match, (size_t)(sizeof(match)/sizeof(match[0])));
    errcode = cre2_error_code(re);

    // step 5: Validate
    checksum = (long)pattern_len + (long)add_ret + (long)compile_ret + (long)match_count + (long)errcode + (long)(compiled_pat != NULL) + (long)(opt != NULL) + (long)(re != NULL) + (long)(set != NULL);
    (void)checksum;

    // step 6: Cleanup
    cre2_set_delete(set);
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}