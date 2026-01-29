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
//<ID> 1301
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(hello|world)\\s+(\\d+)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    char text_buf[] = "greeting: hello 12345 end";
    const int text_len = (int)(sizeof(text_buf) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_string_t match;
    const char * re_pat = NULL;
    int easy_ret = 0;
    int never_nl_flag = 0;
    long computed_check = 0;

    // step 2: Setup
    memset(&match, 0, sizeof(match));
    opt = cre2_opt_new();

    // step 3: Configure
    cre2_opt_set_never_nl(opt, 1);

    // step 4: Core operations
    re = cre2_new(pattern, pattern_len, opt);
    re_pat = cre2_pattern(re);
    easy_ret = cre2_easy_match(pattern, pattern_len, text_buf, text_len, &match, 1);

    // step 5: Validate / compute lightweight checksum
    never_nl_flag = cre2_opt_never_nl(opt);
    computed_check = (long)pattern_len + (long)text_len + (long)easy_ret + (long)(re != NULL)
                     + (long)(re_pat != NULL) + (long)match.length + (long)never_nl_flag + (long)(opt != NULL);
    (void)computed_check;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}