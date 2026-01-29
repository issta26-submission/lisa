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
//<ID> 2171
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(?P<first>[A-Za-z]+)-(?P<num>[0-9]+)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    char text_buf[] = "Alice-123 Bob-456 Carol-789";
    const int text_len = (int)(sizeof(text_buf) - 1);
    const char rewrite_buf[] = "$first:$num";
    const int rewrite_len = (int)(sizeof(rewrite_buf) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_named_groups_iter_t * iter = NULL;
    cre2_string_t text_and_target;
    cre2_string_t rewrite;
    const char * first_named = NULL;
    int first_index = 0;
    int never_capture_flag = 0;
    int word_boundary_flag = 0;
    int replace_ret = 0;
    int num_groups = 0;
    long checksum = 0;

    // step 2: Setup (create and configure options)
    opt = cre2_opt_new();
    cre2_opt_set_never_capture(opt, 1);
    cre2_opt_set_word_boundary(opt, 1);
    cre2_opt_set_case_sensitive(opt, 1);

    // step 3: Read back option flags and compile regexp
    never_capture_flag = cre2_opt_never_capture(opt);
    word_boundary_flag = cre2_opt_word_boundary(opt);
    re = cre2_new(pattern, pattern_len, opt);
    num_groups = cre2_num_capturing_groups(re);

    // step 4: Create named-groups iterator and probe first named group
    iter = cre2_named_groups_iter_new(re);
    (void)cre2_named_groups_iter_next(iter, &first_named, &first_index);

    // step 5: Perform a global replace using the compiled regexp
    text_and_target.data = text_buf;
    text_and_target.length = text_len;
    rewrite.data = rewrite_buf;
    rewrite.length = rewrite_len;
    replace_ret = cre2_global_replace_re(re, &text_and_target, &rewrite);

    // step 6: Validate and Cleanup (combine values and release resources)
    checksum = (long)pattern_len + (long)text_and_target.length + (long)rewrite.length + (long)replace_ret + (long)num_groups + (long)never_capture_flag + (long)word_boundary_flag + (long)(first_named != NULL) + (long)first_index + (long)(opt != NULL) + (long)(re != NULL) + (long)(iter != NULL);
    (void)checksum;
    cre2_named_groups_iter_delete(iter);
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}