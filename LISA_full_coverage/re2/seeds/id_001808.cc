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
//<ID> 1808
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
    const char source_text[] = "xx foo bar yy";
    const int source_text_len = (int)(sizeof(source_text) - 1);
    char work_buf[64];
    char match_buf_storage[128];
    memset(work_buf, 0, sizeof(work_buf));
    memset(match_buf_storage, 0, sizeof(match_buf_storage));
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_named_groups_iter_t * iter = NULL;
    const char * found_name = NULL;
    int found_index = -1;
    bool iter_next_res = false;
    cre2_string_t matches[4];
    memset(matches, 0, sizeof(matches));
    int num_groups = 0;
    int match_res = 0;
    int rev = 0;
    long checksum = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    cre2_opt_set_case_sensitive(opt, 1);
    rev = cre2_version_interface_revision();

    // step 3: Configure
    re = cre2_new(pattern, pattern_len, opt);
    num_groups = cre2_num_capturing_groups(re);
    memcpy(work_buf, source_text, (size_t)source_text_len);
    matches[0].data = work_buf;
    matches[0].length = 0;
    matches[1].data = work_buf;
    matches[1].length = 0;
    matches[2].data = work_buf;
    matches[2].length = 0;
    matches[3].data = match_buf_storage;
    matches[3].length = 0;

    // step 4: Operate
    iter = cre2_named_groups_iter_new(re);
    iter_next_res = cre2_named_groups_iter_next(iter, &found_name, &found_index);
    match_res = cre2_match(re, work_buf, source_text_len, 0, source_text_len, (cre2_anchor_t)0, matches, 4);

    // step 5: Validate
    checksum = (long)pattern_len + (long)source_text_len + (long)rev + (long)num_groups + (long)match_res + (long)(found_index) + (long)(iter_next_res ? 1 : 0) + (long)matches[0].length + (long)matches[1].length;
    (void)checksum;

    // step 6: Cleanup
    cre2_named_groups_iter_delete(iter);
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}