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
//<ID> 1274
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/

int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(?P<num>[0-9]+)abc";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    char text_buf[] = "123abc";
    const int text_len = (int)(sizeof(text_buf) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_string_t match;
    int easy_ret = 0;
    int name_index = 0;
    int prog_size = 0;
    long computed_check = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    memset(&match, 0, sizeof(match));
    match.data = NULL;
    match.length = 0;

    // step 3: Configure
    cre2_opt_set_one_line(opt, 1);

    // step 4: Core operations
    re = cre2_new(pattern, pattern_len, opt);
    if (re) {
        prog_size = cre2_program_size(re);
    } else {
        prog_size = 0;
    }
    easy_ret = cre2_easy_match(pattern, pattern_len, text_buf, text_len, &match, 2);
    if (re) {
        name_index = cre2_find_named_capturing_groups(re, "num");
    } else {
        name_index = -1;
    }

    // step 5: Validate / compute lightweight checksum
    computed_check = (long)pattern_len + (long)text_len + (long)prog_size + (long)easy_ret
                     + (long)name_index + (long)(re != NULL) + (long)(opt != NULL) + (long)match.length;
    (void)computed_check;

    // step 6: Cleanup
    if (re) cre2_delete(re);
    if (opt) cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}