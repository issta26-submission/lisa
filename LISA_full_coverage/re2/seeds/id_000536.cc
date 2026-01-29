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
//<ID> 536
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
    const char subject_buf[] = "xxfoobarbaz foo bar foo";
    const int subject_len = (int)(sizeof(subject_buf) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_string_t subject;
    cre2_string_t matches[4];
    cre2_string_t min_range;
    cre2_string_t max_range;
    int nmatch = 4;
    int partial_rc = 0;
    int consume_rc = 0;
    int possible_rc = 0;
    cre2_encoding_t enc = CRE2_UNKNOWN;
    int computed_check = 0;

    // step 2: Setup
    memset(matches, 0, sizeof(matches));
    memset(&min_range, 0, sizeof(min_range));
    memset(&max_range, 0, sizeof(max_range));
    opt = cre2_opt_new();
    enc = cre2_opt_encoding(opt);
    subject.data = subject_buf;
    subject.length = subject_len;

    // step 3: Create regexp
    re = cre2_new(pattern, pattern_len, opt);

    // step 4: Operate
    partial_rc = cre2_partial_match(pattern, &subject, matches, nmatch);
    consume_rc = cre2_consume_re(re, &subject, matches, nmatch);
    possible_rc = cre2_possible_match_range(re, &min_range, &max_range, 10);

    // step 5: Validate
    computed_check = (int)enc + partial_rc + consume_rc + possible_rc + matches[0].length + matches[1].length + matches[2].length + min_range.length + max_range.length + subject.length;
    (void)computed_check;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}