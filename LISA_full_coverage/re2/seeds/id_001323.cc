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
//<ID> 1323
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(hello)\\s+(world)?";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    char text_buf[] = "greeting: hello   world!";
    const int text_len = (int)(sizeof(text_buf) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_string_t easy_matches[2];
    cre2_string_t partial_input[1];
    cre2_string_t partial_matches[2];
    const char * re_pat = NULL;
    int easy_ret = 0;
    int partial_ret = 0;
    int num_groups = 0;
    long computed_check = 0;

    // step 2: Setup
    memset(easy_matches, 0, sizeof(easy_matches));
    memset(partial_input, 0, sizeof(partial_input));
    memset(partial_matches, 0, sizeof(partial_matches));
    opt = cre2_opt_new();
    cre2_opt_set_word_boundary(opt, 1);

    // step 3: Operate - compile and run matches
    re = cre2_new(pattern, pattern_len, opt);
    re_pat = cre2_pattern(re);
    num_groups = cre2_num_capturing_groups(re);
    partial_input[0].data = text_buf;
    partial_input[0].length = text_len;
    easy_ret = cre2_easy_match(pattern, pattern_len, text_buf, text_len, easy_matches, 2);
    partial_ret = cre2_partial_match(pattern, partial_input, partial_matches, 2);

    // step 4: Validate / compute lightweight checksum
    computed_check = (long)pattern_len + (long)text_len + (long)easy_ret + (long)partial_ret
                     + (long)num_groups + (long)(re != NULL) + (long)(re_pat != NULL)
                     + (long)easy_matches[0].length + (long)partial_matches[0].length;
    (void)computed_check;

    // step 5: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // step 6: Finalize
    // API sequence test completed successfully
    return 66;
}