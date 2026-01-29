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
//<ID> 584
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(foo)(baz)?";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char text_buf[] = "xxfoobazYY";
    const int text_len = (int)(sizeof(text_buf) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_set * set = NULL;
    cre2_string_t subject;
    cre2_string_t matches[3];
    int nmatch = 3;
    cre2_encoding_t enc = CRE2_UNKNOWN;
    int errcode = 0;
    size_t setmatch_count = 0;
    int set_match_ints[4];
    size_t set_match_len = 4;
    int computed_check = 0;

    // step 2: Setup
    memset(matches, 0, sizeof(matches));
    memset(set_match_ints, 0, sizeof(set_match_ints));
    subject.data = text_buf;
    subject.length = text_len;
    opt = cre2_opt_new();

    // step 3: Configure
    enc = cre2_opt_encoding(opt);
    cre2_opt_set_case_sensitive(opt, 1);

    // step 4: Operate
    re = cre2_new(pattern, pattern_len, opt);
    errcode = cre2_error_code(re);
    set = cre2_set_new(opt, (cre2_anchor_t)0);
    cre2_set_add_simple(set, pattern);
    cre2_set_compile(set);
    setmatch_count = cre2_set_match(set, text_buf, (size_t)text_len, set_match_ints, set_match_len);

    // step 5: Validate
    computed_check = (int)enc + errcode + (int)setmatch_count + set_match_ints[0] + (int)matches[0].length + (int)subject.length;
    (void)computed_check;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}