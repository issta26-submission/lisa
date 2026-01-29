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
//<ID> 880
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(foo)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    char text_buf[] = "xx foo yy";
    const int text_len = (int)(sizeof(text_buf) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    const char * pattern_from_re = NULL;
    cre2_string_t input;
    cre2_string_t matches[2];
    cre2_named_groups_iter_t * iter = NULL;
    int find_res = 0;
    long computed_check = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    re = cre2_new(pattern, pattern_len, opt);
    pattern_from_re = cre2_pattern(re);
    memset(matches, 0, sizeof(matches));

    // step 3: Operate
    input.data = text_buf;
    input.length = text_len;
    find_res = cre2_find_and_consume(pattern_from_re, &input, matches, 2);
    iter = cre2_named_groups_iter_new(re);

    // step 4: Validate
    computed_check = (long)find_res + (long)matches[0].length + (long)strlen(pattern_from_re) + (long)input.length;
    (void)computed_check;

    // step 5: Cleanup
    cre2_named_groups_iter_delete(iter);
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}