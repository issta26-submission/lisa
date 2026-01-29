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
//<ID> 530
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
    const char subject_buf[] = "xxfoobarbaz";
    const int subject_len = (int)(sizeof(subject_buf) - 1);
    char consume_buf[] = "foobarbaz and more";
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_string_t subject;
    cre2_string_t consume_subject;
    cre2_string_t matches1[4];
    cre2_string_t matches2[4];
    cre2_string_t min_range;
    cre2_string_t max_range;
    cre2_encoding_t enc;
    int nmatch1 = 4;
    int nmatch2 = 4;
    int partial_rc = 0;
    int consume_rc = 0;
    int possible_rc = 0;
    int computed_check = 0;

    // step 2: Setup
    memset(matches1, 0, sizeof(matches1));
    memset(matches2, 0, sizeof(matches2));
    memset(&min_range, 0, sizeof(min_range));
    memset(&max_range, 0, sizeof(max_range));
    subject.data = subject_buf;
    subject.length = subject_len;
    consume_subject.data = consume_buf;
    consume_subject.length = (int)(sizeof(consume_buf) - 1);
    opt = cre2_opt_new();
    cre2_opt_set_case_sensitive(opt, 1);

    // step 3: Create regexp using options and inspect encoding
    enc = cre2_opt_encoding(opt);
    re = cre2_new(pattern, pattern_len, opt);

    // step 4: Operate: partial match using pattern API, then consume using compiled regex, then possible match range
    partial_rc = cre2_partial_match(pattern, &subject, matches1, nmatch1);
    consume_rc = cre2_consume_re(re, &consume_subject, matches2, nmatch2);
    possible_rc = cre2_possible_match_range(re, &min_range, &max_range, 64);

    // step 5: Validate (combine outputs to create meaningful dataflow)
    computed_check = (int)enc + partial_rc + consume_rc + possible_rc
                     + matches1[0].length + matches1[1].length
                     + matches2[0].length + matches2[1].length
                     + min_range.length + max_range.length + pattern_len + subject.length + consume_subject.length;
    (void)computed_check;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}