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
//<ID> 342
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    cre2_options_t * opt = NULL;
    const char pattern[] = "world";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char text[] = "hello world";
    const int text_len = (int)(sizeof(text) - 1);
    cre2_string_t match[2];
    int match_result = 0;
    long long max_mem_value = 0;
    int computed_check = 0;

    // step 2: Setup
    memset(match, 0, sizeof(match));
    opt = cre2_opt_new();

    // step 3: Configure
    cre2_opt_set_posix_syntax(opt, 1);
    max_mem_value = (long long)cre2_opt_max_mem(opt);

    // step 4: Operate
    match_result = cre2_easy_match(pattern, pattern_len, text, text_len, match, 2);

    // step 5: Validate
    computed_check = match_result + (int)match[0].length + (int)(max_mem_value & 0x7FFFFFFF);
    (void)computed_check;

    // step 6: Cleanup
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}