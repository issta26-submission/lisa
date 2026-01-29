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
//<ID> 534
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
    char consume_buf[] = "foobarbaz";
    const int consume_len = (int)(sizeof(consume_buf) - 1);
    cre2_options_t *opt = NULL;
    cre2_regexp_t *re = NULL;
    cre2_string_t subject;
    cre2_string_t consume_text;
    cre2_string_t matches[3];
    cre2_string_t matches2[3];
    cre2_string_t min_s;
    cre2_string_t max_s;
    char min_buf[32];
    char max_buf[32];
    int nmatch = 3;
    int partial_rc = 0;
    int consume_rc = 0;
    int pmr_rc = 0;
    cre2_encoding_t enc = CRE2_UNKNOWN;
    int computed_check = 0;

    // step 2: Setup
    memset(&subject, 0, sizeof(subject));
    memset(&consume_text, 0, sizeof(consume_text));
    memset(matches, 0, sizeof(matches));
    memset(matches2, 0, sizeof(matches2));
    memset(&min_s, 0, sizeof(min_s));
    memset(&max_s, 0, sizeof(max_s));
    memset(min_buf, 0, sizeof(min_buf));
    memset(max_buf, 0, sizeof(max_buf));
    opt = cre2_opt_new();
    cre2_opt_set_encoding(opt, CRE2_UTF8);
    enc = cre2_opt_encoding(opt);
    subject.data = subject_buf;
    subject.length = subject_len;
    consume_text.data = consume_buf;
    consume_text.length = consume_len;
    min_s.data = min_buf;
    min_s.length = (int)sizeof(min_buf);
    max_s.data = max_buf;
    max_s.length = (int)sizeof(max_buf);

    // step 3: Create regexp
    re = cre2_new(pattern, pattern_len, opt);

    // step 4: Core operations
    partial_rc = cre2_partial_match(pattern, &subject, matches, nmatch);
    consume_rc = cre2_consume_re(re, &consume_text, matches2, nmatch);
    pmr_rc = cre2_possible_match_range(re, &min_s, &max_s, 16);

    // step 5: Validate results
    computed_check = partial_rc + consume_rc + pmr_rc + (int)enc + matches[0].length + matches2[0].length + min_s.length + max_s.length + subject.length + consume_text.length;
    (void)computed_check;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}