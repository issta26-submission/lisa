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
//<ID> 1983
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern1[] = "foo";
    const size_t pattern1_len = (sizeof(pattern1) - 1);
    const char pattern2[] = "bar";
    const size_t pattern2_len = (sizeof(pattern2) - 1);
    const char text[] = "xxfoobarxx foobaz barfoo";
    const size_t text_len = (sizeof(text) - 1);
    cre2_options_t * opt = NULL;
    cre2_set * set = NULL;
    char add_error[128];
    int add_ret1 = 0;
    int add_ret2 = 0;
    int compile_ret = 0;
    int match_array[16];
    size_t match_len = 16;
    size_t match_count = 0;
    long checksum = 0;

    // step 2: Setup
    memset(add_error, 0, sizeof(add_error));
    memset(match_array, 0, sizeof(match_array));
    opt = cre2_opt_new();
    cre2_opt_set_one_line(opt, 1);
    set = cre2_set_new(opt, CRE2_UNANCHORED);

    // step 3: Core operations - add patterns to set
    add_ret1 = cre2_set_add(set, pattern1, pattern1_len, add_error, sizeof(add_error));
    add_ret2 = cre2_set_add(set, pattern2, pattern2_len, add_error, sizeof(add_error));
    compile_ret = cre2_set_compile(set);

    // step 4: Core operations - perform matching against text
    match_count = cre2_set_match(set, text, text_len, match_array, match_len);

    // step 5: Validate
    checksum = (long)pattern1_len + (long)pattern2_len + (long)text_len + (long)add_ret1 + (long)add_ret2 + (long)compile_ret + (long)match_count + (long)match_array[0] + (long)(opt != NULL) + (long)(set != NULL);
    (void)checksum;

    // step 6: Cleanup
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}