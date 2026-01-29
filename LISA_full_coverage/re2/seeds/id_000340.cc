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
//<ID> 340
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(hello) (world)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    char input_buf[] = "hello world and more";
    const int input_len = (int)(sizeof(input_buf) - 1);
    cre2_options_t * opt = NULL;
    cre2_string_t matches[3];
    int match_result = 0;
    long long max_mem_val = 0;
    int computed_check = 0;

    // step 2: Setup
    memset(matches, 0, sizeof(matches));

    // step 3: Configure
    opt = cre2_opt_new();
    cre2_opt_set_posix_syntax(opt, 1);
    max_mem_val = (long long)cre2_opt_max_mem(opt);

    // step 4: Operate
    match_result = cre2_easy_match(pattern, pattern_len, input_buf, input_len, matches, 3);

    // step 5: Validate
    computed_check = match_result + matches[0].length + matches[1].length + matches[2].length + (int)max_mem_val;
    (void)computed_check;

    // step 6: Cleanup
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}