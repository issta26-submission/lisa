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
//<ID> 428
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "^(hello)(world)?$";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char set_pattern[] = "foo|hello|bar";
    const size_t set_pattern_len = (size_t)(sizeof(set_pattern) - 1);
    const char text_buf[] = "helloworld";
    const int text_len = (int)(sizeof(text_buf) - 1);
    cre2_options_t *opt = NULL;
    cre2_regexp_t *re = NULL;
    cre2_set *set = NULL;
    cre2_string_t inputs[1];
    cre2_string_t matches[3];
    int add_res = 0;
    int compile_res = 0;
    int full_res = 0;
    const char *pat_back = NULL;
    int computed_check = 0;

    // step 2: Setup
    memset(matches, 0, sizeof(matches));
    memset(inputs, 0, sizeof(inputs));
    inputs[0].data = text_buf;
    inputs[0].length = text_len;
    opt = cre2_opt_new();
    set = cre2_set_new(opt, CRE2_UNANCHORED);

    // step 3: Configure
    re = cre2_new(pattern, pattern_len, opt);
    add_res = cre2_set_add_simple(set, set_pattern);
    compile_res = cre2_set_compile(set);

    // step 4: Operate
    full_res = cre2_full_match(pattern, inputs, matches, 3);
    pat_back = cre2_pattern(re);

    // step 5: Validate
    computed_check = full_res + compile_res + add_res + matches[0].length + inputs[0].length + pattern_len + (int)pat_back[0];
    (void)computed_check;

    // step 6: Cleanup
    cre2_set_delete(set);
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}