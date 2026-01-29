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
//<ID> 344
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
    cre2_string_t match;
    int match_result = 0;
    int nmatch = 1;
    int64_t mem_limit = (int64_t) (4LL * 1024LL * 1024LL); /* 4MB */
    int64_t observed_max_mem = 0;
    int computed_check = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    cre2_opt_set_posix_syntax(opt, 1);
    cre2_opt_set_max_mem(opt, mem_limit);
    observed_max_mem = cre2_opt_max_mem(opt);

    // step 3: Prepare buffers
    memset(&match, 0, sizeof(match));

    // step 4: Operate
    match_result = cre2_easy_match(pattern, pattern_len, text, text_len, &match, nmatch);

    // step 5: Validate / compute checksum
    computed_check = match_result + (int)observed_max_mem + match.length + pattern_len + text_len;
    (void)computed_check;

    // step 6: Cleanup
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}