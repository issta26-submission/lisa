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
//<ID> 349
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
    cre2_string_t matches[2];
    int easy_result = 0;
    int computed_check = 0;
    int64_t max_mem = 0;

    // step 2: Setup
    memset(matches, 0, sizeof(matches));
    opt = cre2_opt_new();

    // step 3: Configure
    cre2_opt_set_posix_syntax(opt, 1);
    cre2_opt_set_max_mem(opt, (int64_t)1024 * 1024); /* set 1MB cap */
    max_mem = cre2_opt_max_mem(opt);

    // step 4: Operate
    easy_result = cre2_easy_match(pattern, pattern_len, text, text_len, matches, 2);

    // step 5: Validate
    computed_check = easy_result + (int)(pattern_len) + (int)(text_len) + (int)(max_mem & 0xFFFF) + matches[0].length;
    (void)computed_check;

    // step 6: Cleanup
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}