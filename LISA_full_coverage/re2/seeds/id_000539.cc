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
//<ID> 539
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
    const char subject_buf[] = "xxfoobarbaz foo";
    const int subject_len = (int)(sizeof(subject_buf) - 1);
    cre2_options_t *opt = NULL;
    cre2_regexp_t *re = NULL;
    cre2_string_t subject;
    cre2_string_t min_range;
    cre2_string_t max_range;
    cre2_string_t matches_partial[3];
    cre2_string_t matches_consume[3];
    int nmatch_partial = 3;
    int nmatch_consume = 3;
    int rc_partial = 0;
    int rc_consume = 0;
    int rc_range = 0;
    cre2_encoding_t enc = CRE2_UNKNOWN;
    int computed_check = 0;

    // step 2: Setup
    memset(matches_partial, 0, sizeof(matches_partial));
    memset(matches_consume, 0, sizeof(matches_consume));
    memset(&min_range, 0, sizeof(min_range));
    memset(&max_range, 0, sizeof(max_range));
    subject.data = subject_buf;
    subject.length = subject_len;
    opt = cre2_opt_new();

    // step 3: Configure
    enc = cre2_opt_encoding(opt);
    re = cre2_new(pattern, pattern_len, opt);

    // step 4: Operate
    rc_partial = cre2_partial_match(pattern, &subject, matches_partial, nmatch_partial);
    rc_consume = cre2_consume_re(re, &subject, matches_consume, nmatch_consume);
    rc_range = cre2_possible_match_range(re, &min_range, &max_range, 16);

    // step 5: Validate
    computed_check = rc_partial + rc_consume + rc_range + subject.length + matches_partial[0].length + matches_consume[0].length + (int)enc + min_range.length + max_range.length;
    (void)computed_check;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}