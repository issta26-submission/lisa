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
//<ID> 878
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "a.*b";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    char text_buf[] = "a\nb";
    const int text_len = (int)(sizeof(text_buf) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_string_t matches[2];
    int easy_res = 0;
    int num_groups = 0;
    long computed_check = 0;
    const char * pat_from_re = NULL;

    // step 2: Setup
    opt = cre2_opt_new();
    cre2_opt_set_dot_nl(opt, 1);
    cre2_opt_set_case_sensitive(opt, 1);

    // step 3: Core operations
    re = cre2_new(pattern, pattern_len, opt);
    pat_from_re = cre2_pattern(re);
    memset(matches, 0, sizeof(matches));
    easy_res = cre2_easy_match(pattern, pattern_len, text_buf, text_len, matches, 2);
    num_groups = cre2_num_capturing_groups(re);

    // step 4: Validate / compute lightweight checksum
    computed_check = (long)easy_res + (long)num_groups + (long)matches[0].length + (long)pattern_len + (long)text_len + (long)(pat_from_re ? 1 : 0);
    (void)computed_check;

    // step 5: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}