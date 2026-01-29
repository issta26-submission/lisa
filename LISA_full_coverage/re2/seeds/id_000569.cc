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
//<ID> 569
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(fo.o)(bar)?";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char text_buf[] = "xxfoozbarYY";
    const int text_len = (int)(sizeof(text_buf) - 1);
    cre2_options_t *opt = NULL;
    int dot_nl_flag = 0;
    cre2_regexp_t *re = NULL;
    cre2_string_t subject;
    cre2_string_t matches[3];
    int nmatch = 3;
    int rc_pm = 0;
    cre2_set *set = NULL;
    int add_rc = 0;
    size_t setmatch_count = 0;
    int match_ints[4];
    size_t match_len = 4;
    int computed_check = 0;

    // step 2: Setup
    memset(matches, 0, sizeof(matches));
    memset(match_ints, 0, sizeof(match_ints));
    subject.data = text_buf;
    subject.length = text_len;
    opt = cre2_opt_new();

    // step 3: Configure
    dot_nl_flag = cre2_opt_dot_nl(opt);

    // step 4: Operate
    re = cre2_new(pattern, pattern_len, opt);
    rc_pm = cre2_partial_match(pattern, &subject, matches, nmatch);
    set = cre2_set_new(opt, (cre2_anchor_t)0);
    add_rc = cre2_set_add_simple(set, pattern);
    cre2_set_compile(set);
    setmatch_count = cre2_set_match(set, text_buf, (size_t)text_len, match_ints, match_len);

    // step 5: Validate
    computed_check = rc_pm + add_rc + (int)setmatch_count + dot_nl_flag + matches[0].length + matches[1].length + matches[2].length + subject.length;
    (void)computed_check;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}