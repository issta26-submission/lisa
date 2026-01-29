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
//<ID> 2160
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
    char text_buf[] = "Bob42 and Alice123";
    const int text_len = (int)(sizeof(text_buf) - 1);
    const char rewrite_buf[] = "$1:$2";
    const int rewrite_len = (int)(sizeof(rewrite_buf) - 1);
    cre2_string_t text_and_target;
    cre2_string_t rewrite;
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    int posix_flag = 0;
    int groups = 0;
    int prog_size = 0;
    int replace_ret = 0;
    long checksum = 0;

    // step 2: Setup (create and configure options)
    opt = cre2_opt_new();
    cre2_opt_set_encoding(opt, CRE2_UTF8);
    posix_flag = cre2_opt_posix_syntax(opt);

    // step 3: Configure (compile regexp)
    re = cre2_new(pattern, pattern_len, opt);

    // step 4: Operate (inspect compiled regexp and perform a global replace)
    groups = cre2_num_capturing_groups(re);
    prog_size = cre2_program_size(re);
    text_and_target.data = text_buf;
    text_and_target.length = text_len;
    rewrite.data = rewrite_buf;
    rewrite.length = rewrite_len;
    replace_ret = cre2_global_replace(pattern, &text_and_target, &rewrite);

    // step 5: Validate (combine values so results flow through)
    checksum = (long)pattern_len + (long)posix_flag + (long)groups + (long)prog_size + (long)replace_ret + (long)text_and_target.length + (long)rewrite.length + (long)(opt != NULL) + (long)(re != NULL);
    (void)checksum;

    // step 6: Cleanup (release resources)
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}