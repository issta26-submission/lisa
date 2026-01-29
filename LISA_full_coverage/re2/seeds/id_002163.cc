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
//<ID> 2163
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "([A-Za-z]+)([0-9]+)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    char text_buf[] = "Bob42 Alice123 Carol7";
    const int text_len = (int)(sizeof(text_buf) - 1);
    const char rewrite_buf[] = "$1:$2";
    const int rewrite_len = (int)(sizeof(rewrite_buf) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_string_t text_and_target;
    cre2_string_t rewrite;
    int replace_ret = 0;
    int posix_flag = 0;
    int num_groups = 0;
    int prog_size = 0;
    long checksum = 0;

    // step 2: Setup (create and configure options)
    opt = cre2_opt_new();
    cre2_opt_set_encoding(opt, CRE2_UTF8);
    posix_flag = cre2_opt_posix_syntax(opt);

    // step 3: Compile regexp using options
    re = cre2_new(pattern, pattern_len, opt);
    num_groups = cre2_num_capturing_groups(re);
    prog_size = cre2_program_size(re);

    // step 4: Prepare strings and perform global replace (operate)
    memset(&text_and_target, 0, sizeof(text_and_target));
    memset(&rewrite, 0, sizeof(rewrite));
    text_and_target.data = text_buf;
    text_and_target.length = text_len;
    rewrite.data = rewrite_buf;
    rewrite.length = rewrite_len;
    replace_ret = cre2_global_replace(pattern, &text_and_target, &rewrite);

    // step 5: Validate (combine values so results flow through)
    checksum = (long)pattern_len + (long)text_and_target.length + (long)rewrite.length + (long)replace_ret + (long)posix_flag + (long)num_groups + (long)prog_size + (long)(opt != NULL) + (long)(re != NULL);
    (void)checksum;

    // step 6: Cleanup (release resources)
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}