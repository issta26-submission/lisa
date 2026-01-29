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
//<ID> 532
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
    char subject_buf[] = "xxfoobarbaz";
    const int subject_len = (int)(sizeof(subject_buf) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_string_t subject;
    cre2_string_t matches[3];
    cre2_string_t min_range;
    cre2_string_t max_range;
    int nmatch = 3;
    int maxlen = 16;
    int rc_partial = 0;
    int rc_consume = 0;
    int rc_range = 0;
    cre2_encoding_t enc = CRE2_UNKNOWN;
    int computed_check = 0;

    // step 2: Setup
    memset(matches, 0, sizeof(matches));
    memset(&min_range, 0, sizeof(min_range));
    memset(&max_range, 0, sizeof(max_range));
    subject.data = subject_buf;
    subject.length = subject_len;
    opt = cre2_opt_new();
    cre2_opt_set_encoding(opt, CRE2_UTF8);

    // step 3: Create compiled regexp with options and query encoding
    re = cre2_new(pattern, pattern_len, opt);
    enc = cre2_opt_encoding(opt);

    // step 4: Operate: use pattern-based partial match, then consume using compiled regexp, then compute possible match range
    rc_partial = cre2_partial_match(pattern, &subject, matches, nmatch);
    rc_consume = cre2_consume_re(re, &subject, matches, nmatch);
    rc_range = cre2_possible_match_range(re, &min_range, &max_range, maxlen);

    // step 5: Validate (combine returned values and lengths for a meaningful flow)
    computed_check = (int)enc + rc_partial + rc_consume + rc_range + subject.length + matches[0].length + min_range.length + max_range.length;
    (void)computed_check;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}