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
//<ID> 1806
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(?P<name>foo)(bar)?";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char source_text[] = "prefix foobar suffix";
    const int source_text_len = (int)(sizeof(source_text) - 1);
    char work_buf[128];
    char match_buf[64];
    memset(work_buf, 0, sizeof(work_buf));
    memset(match_buf, 0, sizeof(match_buf));
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_named_groups_iter_t * iter = NULL;
    cre2_string_t matches[2];
    memset(matches, 0, sizeof(matches));
    int rev = 0;
    int num_caps = 0;
    int match_res = 0;
    long checksum = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    cre2_opt_set_case_sensitive(opt, 1);
    rev = cre2_version_interface_revision();
    re = cre2_new(pattern, pattern_len, opt);

    // step 3: Configure
    num_caps = cre2_num_capturing_groups(re);
    iter = cre2_named_groups_iter_new(re);
    memcpy(work_buf, source_text, (size_t)source_text_len);
    matches[0].data = work_buf;
    matches[0].length = 0;
    matches[1].data = work_buf;
    matches[1].length = 0;

    // step 4: Operate
    match_res = cre2_match(re, work_buf, source_text_len, 0, source_text_len, (cre2_anchor_t)0, matches, 2);

    // step 5: Validate
    checksum = (long)pattern_len + (long)source_text_len + (long)rev + (long)num_caps + (long)match_res + (long)matches[0].length + (long)(opt != NULL) + (long)(re != NULL) + (long)(iter != NULL);
    (void)checksum;

    // step 6: Cleanup
    cre2_named_groups_iter_delete(iter);
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}