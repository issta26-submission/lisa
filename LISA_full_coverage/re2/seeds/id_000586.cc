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
//<ID> 586
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
    const char text_buf[] = "xxfoobarYY";
    const int text_len = (int)(sizeof(text_buf) - 1);
    cre2_options_t *opt = NULL;
    cre2_encoding_t encoding = CRE2_UNKNOWN;
    cre2_regexp_t *re = NULL;
    cre2_set *set = NULL;
    cre2_string_t subject;
    cre2_string_t matches[3];
    int nmatch = 3;
    int match_ints[4];
    size_t setmatch_count = 0;
    int set_add_rc = 0;
    int set_compile_rc = 0;
    int rc_easy = 0;
    int errcode = 0;
    int computed_check = 0;

    // step 2: Setup
    memset(matches, 0, sizeof(matches));
    memset(match_ints, 0, sizeof(match_ints));
    subject.data = text_buf;
    subject.length = text_len;
    opt = cre2_opt_new();

    // step 3: Configure
    cre2_opt_set_case_sensitive(opt, 1);
    encoding = cre2_opt_encoding(opt);

    // step 4: Operate
    re = cre2_new(pattern, pattern_len, opt);
    errcode = cre2_error_code(re);
    set = cre2_set_new(opt, (cre2_anchor_t)0);
    set_add_rc = cre2_set_add_simple(set, pattern);
    set_compile_rc = cre2_set_compile(set);
    setmatch_count = cre2_set_match(set, text_buf, (size_t)text_len, match_ints, (size_t)4);
    rc_easy = cre2_easy_match(pattern, pattern_len, text_buf, text_len, matches, nmatch);

    // step 5: Validate
    computed_check = errcode + (int)encoding + rc_easy + set_add_rc + set_compile_rc + (int)setmatch_count + matches[0].length + subject.length;
    (void)computed_check;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}