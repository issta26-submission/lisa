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
//<ID> 607
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(foo)(bar)?(baz)?";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char text_buf[] = "xxfoobarbazYY";
    const int text_len = (int)(sizeof(text_buf) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_string_t subject;
    cre2_string_t matches[16];
    int version_cur = 0;
    int perl_flag = 0;
    int num_groups = 0;
    int nmatch = 0;
    int match_rc = 0;
    int computed_check = 0;

    // step 2: Setup
    memset(matches, 0, sizeof(matches));
    subject.data = text_buf;
    subject.length = text_len;
    opt = cre2_opt_new();
    cre2_opt_set_literal(opt, 1);
    perl_flag = cre2_opt_perl_classes(opt);
    version_cur = cre2_version_interface_current();

    // step 3: Configure
    re = cre2_new(pattern, pattern_len, opt);
    num_groups = cre2_num_capturing_groups(re);
    nmatch = num_groups + 1;

    // step 4: Operate
    match_rc = cre2_match(re, subject.data, subject.length, 0, subject.length, (cre2_anchor_t)0, matches, nmatch);

    // step 5: Validate
    computed_check = version_cur + perl_flag + num_groups + match_rc + (int)matches[0].length + subject.length;
    (void)computed_check;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}