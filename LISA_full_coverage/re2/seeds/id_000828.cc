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
//<ID> 828
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(ab)c";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    char text_buf[] = "xxabcz";
    const int text_len = (int)(sizeof(text_buf) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_encoding_t enc = CRE2_UNKNOWN;
    cre2_string_t match_slots[3];
    cre2_string_t partial_subject;
    cre2_string_t partial_slots[2];
    int match_res = 0;
    int partial_res = 0;
    long computed_check = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    cre2_opt_set_encoding(opt, CRE2_UTF8);
    enc = cre2_opt_encoding(opt);

    // step 3: Core operations - compile and run full match
    re = cre2_new(pattern, pattern_len, opt);
    memset(match_slots, 0, sizeof(match_slots));
    match_res = cre2_match(re, text_buf, text_len, 0, text_len, CRE2_UNANCHORED, match_slots, 3);

    // step 4: Core operations - use non-re partial match API on same text
    memset(&partial_subject, 0, sizeof(partial_subject));
    partial_subject.data = text_buf + 2;
    partial_subject.length = text_len - 2;
    memset(partial_slots, 0, sizeof(partial_slots));
    partial_res = cre2_partial_match(pattern, &partial_subject, partial_slots, 2);

    // step 5: Validate (compute a checksum-like value to tie results together)
    computed_check = (long)enc + (long)match_res + (long)partial_res + (long)match_slots[0].length + (long)partial_slots[0].length + (long)pattern_len;
    (void)computed_check;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}