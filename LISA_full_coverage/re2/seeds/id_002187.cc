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
//<ID> 2187
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "([A-Za-z]+)-([0-9]+)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    char text_buf[] = "Alice-123 Bob-456 Carol-789";
    const int text_len = (int)(sizeof(text_buf) - 1);
    char target_buf[64];
    const int target_buf_len = (int)(sizeof(target_buf));
    cre2_string_t text_and_target;
    cre2_string_t target;
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    const char * pattern_from_re = NULL;
    int perl_flag = 0;
    int consume_ret = 0;
    long checksum = 0;

    // step 2: Setup
    memset(&text_and_target, 0, sizeof(text_and_target));
    memset(&target, 0, sizeof(target));
    memset(target_buf, 0, (size_t)target_buf_len);
    opt = cre2_opt_new();
    cre2_opt_set_perl_classes(opt, 1);
    perl_flag = cre2_opt_perl_classes(opt);

    // step 3: Configure
    re = cre2_new(pattern, pattern_len, opt);
    pattern_from_re = cre2_pattern(re);
    text_and_target.data = text_buf;
    text_and_target.length = text_len;
    target.data = target_buf;
    target.length = 0;

    // step 4: Operate
    consume_ret = cre2_consume(pattern_from_re, &text_and_target, &target, 2);

    // step 5: Validate
    checksum = (long)pattern_len + (long)consume_ret + (long)perl_flag + (long)text_and_target.length + (long)target.length + (long)(opt != NULL) + (long)(re != NULL);
    (void)checksum;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}