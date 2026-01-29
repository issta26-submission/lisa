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
//<ID> 312
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "^(Hello)(World)$";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    char buffer[64];
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_named_groups_iter_t * iter = NULL;
    const char * err = NULL;
    const char * ng_name = NULL;
    int ng_index = -1;
    int one_line_flag = 1;
    int case_sensitive_flag = 1;

    // step 2: Setup
    memset(buffer, 0, sizeof(buffer));
    opt = cre2_opt_new();

    // step 3: Configure / Create
    cre2_opt_set_one_line(opt, one_line_flag);
    cre2_opt_set_case_sensitive(opt, case_sensitive_flag);
    re = cre2_new(pattern, pattern_len, opt);

    // step 4: Operate / Validate
    err = cre2_error_string(re);
    buffer[0] = (char)((unsigned char)buffer[0] + (unsigned char)((err ? (int)err[0] : 0) & 0x7));
    iter = cre2_named_groups_iter_new(re);
    (void)cre2_named_groups_iter_next(iter, &ng_name, &ng_index);
    buffer[1] = (char)((unsigned char)buffer[1] + (unsigned char)((ng_index & 0xFF) + (ng_name ? (int)ng_name[0] : 0)));
    buffer[2] = (char)((unsigned char)buffer[2] + (unsigned char)((cre2_opt_one_line(opt) & 0x1) + (cre2_opt_case_sensitive(opt) & 0x1)));

    // step 5: Cleanup
    cre2_named_groups_iter_delete(iter);
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}