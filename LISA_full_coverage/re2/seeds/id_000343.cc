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
//<ID> 343
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "world";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char text[] = "hello world!";
    const int text_len = (int)(sizeof(text) - 1);
    cre2_options_t * opt = NULL;
    cre2_string_t match_arr[2];
    int match_result = 0;
    int64_t max_mem_val = 0;
    int computed_check = 0;

    // step 2: Setup
    memset(match_arr, 0, sizeof(match_arr));
    opt = cre2_opt_new();
    cre2_opt_set_posix_syntax(opt, 1);

    // step 3: Operate
    match_result = cre2_easy_match(pattern, pattern_len, text, text_len, match_arr, 2);
    max_mem_val = cre2_opt_max_mem(opt);

    // step 4: Validate
    computed_check = match_result + match_arr[0].length + (int)(max_mem_val & 0x7fffffff);
    (void)computed_check;

    // step 5: Cleanup
    cre2_opt_delete(opt);

    // step 6: API sequence test completed successfully
    return 66;
}