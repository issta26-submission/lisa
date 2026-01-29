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
//<ID> 346
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "hello";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char text[] = "hello world";
    const int text_len = (int)(sizeof(text) - 1);
    cre2_options_t * opt = NULL;
    cre2_string_t matches[1];
    int posix_flag = 1;
    int easy_result = 0;
    int64_t max_mem_before = 0;
    int64_t computed_check = 0;

    // step 2: Setup
    memset(matches, 0, sizeof(matches));
    matches[0].data = NULL;
    matches[0].length = 0;
    opt = cre2_opt_new();

    // step 3: Configure
    cre2_opt_set_posix_syntax(opt, posix_flag);

    // step 4: Operate
    easy_result = cre2_easy_match(pattern, pattern_len, text, text_len, matches, 1);
    max_mem_before = cre2_opt_max_mem(opt);

    // step 5: Validate
    computed_check = (int64_t)easy_result + (int64_t)matches[0].length + max_mem_before;
    (void)computed_check;

    // step 6: Cleanup
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}