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
//<ID> 533
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(a+)(b*)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char subject_buf[] = "aaabbbxx";
    const int subject_len = (int)(sizeof(subject_buf) - 1);
    char text_and_target_buf[] = "aaabbbcc aaabbb";
    const int text_and_target_len = (int)(sizeof(text_and_target_buf) - 1);
    cre2_options_t *opt = NULL;
    cre2_regexp_t *re = NULL;
    cre2_string_t subject;
    cre2_string_t text_and_target;
    cre2_string_t matches[4];
    cre2_string_t minstr;
    cre2_string_t maxstr;
    int nmatch = 4;
    int partial_rc = 0;
    int consume_rc = 0;
    int possible_rc = 0;
    cre2_encoding_t encoding = CRE2_UNKNOWN;
    int computed_check = 0;

    // step 2: Setup
    memset(matches, 0, sizeof(matches));
    memset(&minstr, 0, sizeof(minstr));
    memset(&maxstr, 0, sizeof(maxstr));
    opt = cre2_opt_new();
    subject.data = subject_buf;
    subject.length = subject_len;
    text_and_target.data = text_and_target_buf;
    text_and_target.length = text_and_target_len;

    // step 3: Configure
    cre2_opt_set_encoding(opt, CRE2_UTF8);
    encoding = cre2_opt_encoding(opt);

    // step 4: Create regexp and perform a partial match using the pattern API
    re = cre2_new(pattern, pattern_len, opt);
    partial_rc = cre2_partial_match(pattern, &subject, matches, nmatch);

    // step 5: Use compiled regexp to consume and compute possible match range
    consume_rc = cre2_consume_re(re, &text_and_target, matches, nmatch);
    possible_rc = cre2_possible_match_range(re, &minstr, &maxstr, 16);

    // step 6: Validate and Cleanup
    computed_check = (int)encoding + partial_rc + consume_rc + possible_rc + matches[0].length + matches[1].length + minstr.length + maxstr.length + subject.length + text_and_target.length + pattern_len;
    (void)computed_check;
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}