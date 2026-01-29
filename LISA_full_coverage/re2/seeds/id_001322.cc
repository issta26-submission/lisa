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
//<ID> 1322
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(foo)(bar)?";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    char text_buf[] = "xxfoobarxx";
    const int text_len = (int)(sizeof(text_buf) - 1);
    cre2_options_t *opt = NULL;
    cre2_regexp_t *re = NULL;
    int groups = 0;
    cre2_string_t *matches = NULL;
    cre2_string_t input_strings[1];
    cre2_string_t partial_out[1];
    int easy_ret = 0;
    int partial_ret = 0;
    long computed_check = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    input_strings[0].data = text_buf;
    input_strings[0].length = text_len;
    memset(partial_out, 0, sizeof(partial_out));

    // step 3: Configure
    cre2_opt_set_word_boundary(opt, 1);

    // step 4: Core operations
    re = cre2_new(pattern, pattern_len, opt);
    groups = cre2_num_capturing_groups(re);
    matches = (cre2_string_t *)malloc((size_t)(groups + 1) * sizeof(cre2_string_t));
    memset(matches, 0, (size_t)(groups + 1) * sizeof(cre2_string_t));
    easy_ret = cre2_easy_match(pattern, pattern_len, text_buf, text_len, matches, groups + 1);
    partial_ret = cre2_partial_match(pattern, input_strings, partial_out, 1);

    // step 5: Validate / compute lightweight checksum
    computed_check = (long)pattern_len + (long)text_len + (long)groups + (long)easy_ret + (long)partial_ret
                     + (long)(re != NULL) + (long)(opt != NULL) + (long)matches[0].length + (long)partial_out[0].length;
    (void)computed_check;

    // step 6: Cleanup
    free(matches);
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}