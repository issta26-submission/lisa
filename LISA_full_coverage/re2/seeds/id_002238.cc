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
//<ID> 2238
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "foo|bar";
    char text_buf[] = "xxx foo yyy bar zzz";
    const size_t text_len = (size_t)(sizeof(text_buf) - 1);
    cre2_options_t * opt = NULL;
    struct cre2_set * set = NULL;
    int match_buf[8];
    size_t match_count = 0;
    int set_add_ret = 0;
    int set_compile_ret = 0;
    int literal_flag = 0;
    int64_t max_mem = 0;
    long checksum = 0;

    // step 2: Setup (initialize options and configure)
    opt = cre2_opt_new();
    cre2_opt_set_case_sensitive(opt, 1);
    cre2_opt_set_literal(opt, 0);

    // step 3: Inspect option state (use required getters)
    literal_flag = cre2_opt_literal(opt);
    max_mem = cre2_opt_max_mem(opt);

    // step 4: Build and compile a set using the options
    set = cre2_set_new(opt, (cre2_anchor_t)0);
    set_add_ret = cre2_set_add_simple(set, pattern);
    set_compile_ret = cre2_set_compile(set);

    // step 5: Prepare match buffer and perform matching with the compiled set
    memset(match_buf, 0, sizeof(match_buf));
    match_count = cre2_set_match(set, text_buf, text_len, match_buf, (size_t)(sizeof(match_buf) / sizeof(match_buf[0])));

    // step 6: Validate and cleanup
    checksum = (long)match_count + (long)match_buf[0] + (long)literal_flag + (long)max_mem + (long)set_add_ret + (long)set_compile_ret + (long)(opt != NULL) + (long)(set != NULL);
    (void)checksum;
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}