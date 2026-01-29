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
//<ID> 2145
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(?P<word>\\w+)-(\\d+)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    char subject_buf[] = "Alice-99 extra";
    const int subject_len = (int)(sizeof(subject_buf) - 1);
    cre2_options_t * opt = NULL;
    int easy_ret = 0;
    int partial_ret = 0;
    long checksum = 0;
    cre2_string_t subject = { subject_buf, subject_len };
    cre2_string_t matches_easy[3];
    cre2_string_t matches_partial[3];

    // step 2: Setup (create and configure options)
    opt = cre2_opt_new();
    cre2_opt_set_case_sensitive(opt, 1);
    cre2_opt_set_literal(opt, 0);
    cre2_opt_set_word_boundary(opt, 1);

    // step 3: Operate (prepare match arrays and perform easy match)
    memset(matches_easy, 0, sizeof(matches_easy));
    easy_ret = cre2_easy_match(pattern, pattern_len, subject_buf, subject_len, matches_easy, 3);

    // step 4: Operate (prepare another match array and perform partial match)
    memset(matches_partial, 0, sizeof(matches_partial));
    partial_ret = cre2_partial_match(pattern, &subject, matches_partial, 3);

    // step 5: Validate (combine values so results flow through)
    int literal_flag = cre2_opt_literal(opt);
    int case_flag = cre2_opt_case_sensitive(opt);
    checksum = (long)pattern_len + (long)easy_ret + (long)partial_ret + (long)literal_flag + (long)case_flag + (long)subject.length;
    checksum += (long)matches_easy[0].length + (long)matches_easy[1].length + (long)matches_easy[2].length;
    checksum += (long)matches_partial[0].length + (long)matches_partial[1].length + (long)matches_partial[2].length;
    (void)checksum;

    // step 6: Cleanup (release resources)
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}