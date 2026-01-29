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
//<ID> 2237
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "([A-Za-z]+)-([0-9]+)";
    const size_t text_len = (size_t)(sizeof("Alice-123 Bob-456 Carol-789") - 1);
    char text_buf[] = "Alice-123 Bob-456 Carol-789";
    cre2_options_t * opt = NULL;
    cre2_set * set = NULL;
    int set_add_ret = 0;
    int set_compile_ret = 0;
    int match[16];
    size_t match_count = 0;
    int literal_flag = 0;
    int64_t max_mem_val = 0;
    long checksum = 0;

    // step 2: Setup (create and configure options)
    memset(match, 0, sizeof(match));
    opt = cre2_opt_new();
    cre2_opt_set_case_sensitive(opt, 1);
    cre2_opt_set_literal(opt, 1);
    cre2_opt_set_max_mem(opt, (int64_t)1024 * 1024);

    // step 3: Inspect option values (use required getters)
    literal_flag = cre2_opt_literal(opt);
    max_mem_val = cre2_opt_max_mem(opt);

    // step 4: Build and compile a set of patterns
    set = cre2_set_new(opt, (cre2_anchor_t)0);
    set_add_ret = cre2_set_add_simple(set, pattern);
    set_compile_ret = cre2_set_compile(set);

    // step 5: Operate (match the set against text)
    match_count = cre2_set_match(set, text_buf, text_len, match, (size_t)(sizeof(match) / sizeof(match[0])));

    // step 6: Validate and Cleanup
    checksum = (long)text_len + (long)set_add_ret + (long)set_compile_ret + (long)match_count + (long)literal_flag + (long)max_mem_val + (long)(match[0]);
    (void)checksum;
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}