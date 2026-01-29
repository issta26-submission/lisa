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
//<ID> 348
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // Step 1: Declarations
    const char pattern[] = "hello (world)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char text[] = "hello world and more";
    const int text_len = (int)(sizeof(text) - 1);
    cre2_string_t matches[2];
    cre2_options_t * opt = NULL;
    int match_result = 0;
    int64_t max_mem = 0;
    int computed_check = 0;

    // Step 2: Setup
    memset(matches, 0, sizeof(matches));
    opt = cre2_opt_new();

    // Step 3: Configure
    cre2_opt_set_posix_syntax(opt, 1);
    cre2_opt_set_max_mem(opt, (int64_t)(4LL * 1024LL * 1024LL));

    // Step 4: Operate
    match_result = cre2_easy_match(pattern, pattern_len, text, text_len, matches, 2);

    // Step 5: Validate
    max_mem = cre2_opt_max_mem(opt);
    computed_check = match_result + matches[0].length + (int)(max_mem & 0xFF);
    (void)computed_check;

    // Step 6: Cleanup
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}