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
//<ID> 1303
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(hello|world)\\s+([0-9]+)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    char text_buf[] = "start hello 123 end";
    const int text_len = (int)(sizeof(text_buf) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_string_t matches[3];
    int easy_match_ret = 0;
    const char * re_pat = NULL;
    int never_nl_flag = 0;
    long computed_check = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    memset(matches, 0, sizeof(matches));
    matches[0].data = NULL;
    matches[0].length = 0;
    matches[1].data = NULL;
    matches[1].length = 0;
    matches[2].data = NULL;
    matches[2].length = 0;

    // step 3: Configure
    never_nl_flag = cre2_opt_never_nl(opt);

    // step 4: Core operations
    re = cre2_new(pattern, pattern_len, opt);
    easy_match_ret = cre2_easy_match(pattern, pattern_len, text_buf, text_len, matches, 3);
    re_pat = cre2_pattern(re);

    // step 5: Validate / compute lightweight checksum
    computed_check = (long)pattern_len + (long)text_len + (long)easy_match_ret + (long)(re != NULL)
                     + (long)(re_pat != NULL) + (long)never_nl_flag + (long)matches[0].length + (long)matches[1].length;
    (void)computed_check;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}