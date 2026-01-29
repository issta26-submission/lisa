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
//<ID> 538
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(ab)(c)?";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char subject_buf1[] = "xxabcYY";
    const char subject_buf2[] = "abcabc";
    const int subject_len1 = (int)(sizeof(subject_buf1) - 1);
    const int subject_len2 = (int)(sizeof(subject_buf2) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_string_t subject1;
    cre2_string_t subject2;
    cre2_string_t matches1[4];
    cre2_string_t matches2[4];
    cre2_string_t min_range;
    cre2_string_t max_range;
    int nmatch = 4;
    int partial_rc = 0;
    int consume_rc = 0;
    int range_rc = 0;
    cre2_encoding_t enc = CRE2_UNKNOWN;
    int computed_check = 0;

    // step 2: Setup
    memset(matches1, 0, sizeof(matches1));
    memset(matches2, 0, sizeof(matches2));
    memset(&min_range, 0, sizeof(min_range));
    memset(&max_range, 0, sizeof(max_range));
    opt = cre2_opt_new();
    cre2_opt_set_encoding(opt, CRE2_UTF8);
    enc = cre2_opt_encoding(opt);
    subject1.data = subject_buf1;
    subject1.length = subject_len1;
    subject2.data = subject_buf2;
    subject2.length = subject_len2;

    // step 3: Create regexp
    re = cre2_new(pattern, pattern_len, opt);

    // step 4: Core operations
    partial_rc = cre2_partial_match(pattern, &subject1, matches1, nmatch);
    consume_rc = cre2_consume_re(re, &subject2, matches2, nmatch);
    range_rc = cre2_possible_match_range(re, &min_range, &max_range, 16);

    // step 5: Validate / combine values
    computed_check = (int)enc + partial_rc + consume_rc + range_rc + min_range.length + max_range.length + matches1[0].length + matches2[0].length + subject1.length + subject2.length;
    (void)computed_check;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}