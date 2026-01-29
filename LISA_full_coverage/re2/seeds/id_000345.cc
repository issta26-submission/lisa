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
//<ID> 345
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "hello (world)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char text[] = "hello world and beyond";
    const int text_len = (int)(sizeof(text) - 1);
    cre2_options_t * opt = NULL;
    cre2_string_t matches[2];
    int match_result = 0;
    int computed_check = 0;
    int64_t max_mem = 0;

    // step 2: Setup
    memset(matches, 0, sizeof(matches));
    opt = cre2_opt_new();
    cre2_opt_set_posix_syntax(opt, 1);

    // step 3: Operate
    match_result = cre2_easy_match(pattern, pattern_len, text, text_len, matches, 2);

    // step 4: Validate
    max_mem = cre2_opt_max_mem(opt);
    computed_check = match_result + (int)max_mem + matches[0].length;
    (void)computed_check;

    // step 5: Cleanup
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}