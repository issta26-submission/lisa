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
//<ID> 2135
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(?P<name>[A-Za-z]+)-(\\d{1,5})";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_set * set = NULL;
    cre2_named_groups_iter_t * iter = NULL;
    long checksum = 0;

    char subject_buf[] = "Bob-42";
    int subject_len = (int)(sizeof(subject_buf) - 1);
    cre2_string_t subject = { subject_buf, subject_len };

    char match0_buf[32];
    char match1_buf[32];
    cre2_string_t matches[3];
    matches[0].data = match0_buf;
    matches[0].length = 0;
    matches[1].data = match1_buf;
    matches[1].length = 0;
    matches[2].data = NULL;
    matches[2].length = 0;

    char tat_buf[128];
    memset(tat_buf, 0, sizeof(tat_buf));
    const char tat_init[] = "Alice-123 Carol-456";
    int tat_init_len = (int)(sizeof(tat_init) - 1);
    memcpy(tat_buf, tat_init, (size_t)tat_init_len);
    cre2_string_t text_and_target = { tat_buf, tat_init_len };

    char rewrite_buf[] = "$1:$2";
    cre2_string_t rewrite = { rewrite_buf, (int)(sizeof(rewrite_buf) - 1) };

    int full_match_ret = 0;
    int replace_ret = 0;
    size_t set_add_simple_ret = 0;
    int64_t maxmem = 0;

    // step 2: Setup (create options and inspect defaults)
    opt = cre2_opt_new();
    cre2_opt_set_perl_classes(opt, 1);
    cre2_opt_set_case_sensitive(opt, 1);
    maxmem = cre2_opt_max_mem(opt);

    // step 3: Configure (create a set, add simple pattern, compile regexp and make iterator)
    set = cre2_set_new(opt, (cre2_anchor_t)0);
    set_add_simple_ret = (size_t)cre2_set_add_simple(set, pattern);
    re = cre2_new(pattern, pattern_len, opt);
    iter = cre2_named_groups_iter_new(re);

    // step 4: Operate (perform a full match to populate match buffers and then a replace using compiled regexp)
    memset(match0_buf, 0, sizeof(match0_buf));
    memset(match1_buf, 0, sizeof(match1_buf));
    full_match_ret = cre2_full_match(pattern, &subject, matches, 2);
    replace_ret = cre2_replace_re(re, &text_and_target, &rewrite);

    // step 5: Validate (combine results so values flow through)
    checksum = (long)pattern_len + (long)subject.length + (long)full_match_ret + (long)replace_ret + (long)maxmem + (long)set_add_simple_ret + (long)(re != NULL) + (long)(set != NULL) + (long)(iter != NULL) + (long)text_and_target.length;
    (void)checksum;

    // step 6: Cleanup (release resources)
    cre2_named_groups_iter_delete(iter);
    cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}