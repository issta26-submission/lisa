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
//<ID> 531
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
    const char subject_buf[] = "xxabc yyab zz";
    const int subject_len = (int)(sizeof(subject_buf) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_string_t subject;
    cre2_string_t matches[4];
    cre2_string_t min_range;
    cre2_string_t max_range;
    int nmatch = 4;
    cre2_encoding_t enc;
    int rc_partial = 0;
    int rc_consume = 0;
    int rc_range = 0;
    int computed_check = 0;

    // step 2: Setup
    memset(matches, 0, sizeof(matches));
    memset(&min_range, 0, sizeof(min_range));
    memset(&max_range, 0, sizeof(max_range));
    subject.data = subject_buf;
    subject.length = subject_len;
    opt = cre2_opt_new();

    // step 3: Configure
    cre2_opt_set_perl_classes(opt, 1);
    cre2_opt_set_case_sensitive(opt, 1);
    enc = cre2_opt_encoding(opt);

    // step 4: Create compiled regexp
    re = cre2_new(pattern, pattern_len, opt);

    // step 5: Operate (partial match using pattern, consume using compiled re, then compute possible match range)
    rc_partial = cre2_partial_match(pattern, &subject, matches, nmatch);
    rc_consume = cre2_consume_re(re, &subject, matches, nmatch);
    rc_range = cre2_possible_match_range(re, &min_range, &max_range, 64);

    // step 6: Validate and Cleanup
    computed_check = (int)enc + rc_partial + rc_consume + rc_range + matches[0].length + matches[1].length + min_range.length + max_range.length + subject.length + pattern_len;
    (void)computed_check;
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}