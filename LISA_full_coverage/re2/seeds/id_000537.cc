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
//<ID> 537
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
    const char consume_buf[] = "foobarbaz remaining";
    const int consume_len = (int)(sizeof(consume_buf) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_string_t subject;
    cre2_string_t to_consume;
    cre2_string_t matches1[4];
    cre2_string_t matches2[4];
    cre2_string_t min_range;
    cre2_string_t max_range;
    int nmatch = 4;
    int partial_rc = 0;
    int consume_rc = 0;
    int possible_rc = 0;
    cre2_encoding_t enc = CRE2_UNKNOWN;
    int computed_check = 0;

    // step 2: Setup
    memset(matches1, 0, sizeof(matches1));
    memset(matches2, 0, sizeof(matches2));
    memset(&min_range, 0, sizeof(min_range));
    memset(&max_range, 0, sizeof(max_range));
    opt = cre2_opt_new();
    cre2_opt_set_case_sensitive(opt, 1);
    enc = cre2_opt_encoding(opt);
    subject.data = subject_buf;
    subject.length = subject_len;
    to_consume.data = consume_buf;
    to_consume.length = consume_len;

    // step 3: Create regexp
    re = cre2_new(pattern, pattern_len, opt);

    // step 4: Operate
    partial_rc = cre2_partial_match(pattern, &subject, matches1, nmatch);
    consume_rc = cre2_consume_re(re, &to_consume, matches2, nmatch);
    possible_rc = cre2_possible_match_range(re, &min_range, &max_range, 32);

    // step 5: Validate
    computed_check = partial_rc + consume_rc + possible_rc + matches1[0].length + matches1[1].length + matches2[0].length + matches2[1].length + (int)enc + min_range.length + max_range.length;
    (void)computed_check;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}