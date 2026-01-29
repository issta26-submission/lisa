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
//<ID> 512
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(foo|bar).*";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char text_buf[] = "xx foobar\nbaz";
    const int text_len = (int)(sizeof(text_buf) - 1);
    cre2_options_t *opt = NULL;
    cre2_regexp_t *re = NULL;
    cre2_string_t match_arr[2];
    int nmatch = 2;
    int match_ret = 0;
    int prog_size = 0;
    int computed_check = 0;

    // step 2: Setup
    memset(match_arr, 0, sizeof(match_arr));
    opt = cre2_opt_new();

    // step 3: Configure
    cre2_opt_set_dot_nl(opt, 1);

    // step 4: Operate
    re = cre2_new(pattern, pattern_len, opt);
    prog_size = cre2_program_size(re);
    match_ret = cre2_easy_match(pattern, pattern_len, text_buf, text_len, match_arr, nmatch);

    // step 5: Validate
    computed_check = prog_size + match_ret + match_arr[0].length + match_arr[1].length + pattern_len;
    (void)computed_check;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}