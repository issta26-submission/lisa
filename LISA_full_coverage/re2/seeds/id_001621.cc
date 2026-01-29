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
//<ID> 1621
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "\\bfoo\\b(bar)?";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char text[] = "foo foobar baz";
    const int text_len = (int)(sizeof(text) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_set * set = NULL;
    const char * compiled_pat = NULL;
    int err_code = 0;
    int add_result = 0;
    int compile_result = 0;
    size_t set_matches = 0;
    int matches[4];
    long checksum = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    cre2_opt_set_word_boundary(opt, 1);

    // step 3: Configure
    re = cre2_new(pattern, pattern_len, opt);
    compiled_pat = cre2_pattern(re);
    err_code = cre2_error_code(re);
    set = cre2_set_new(opt, CRE2_UNANCHORED);

    // step 4: Operate
    add_result = cre2_set_add_simple(set, pattern);
    compile_result = cre2_set_compile(set);
    memset(matches, 0, sizeof(matches));
    set_matches = cre2_set_match(set, text, (size_t)text_len, matches, (size_t)4);

    // step 5: Validate
    checksum = (long)pattern_len + (long)text_len + (long)(compiled_pat != NULL) + (long)err_code + (long)add_result + (long)compile_result + (long)set_matches + (long)matches[0];
    (void)checksum;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}