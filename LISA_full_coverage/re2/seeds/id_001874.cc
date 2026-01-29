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
//<ID> 1874
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // Step 1: Declarations
    const char pattern[] = "(a+)(b?)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char source_text[] = "aaab";
    const int source_text_len = (int)(sizeof(source_text) - 1);
    char text_buf[64];
    memset(text_buf, 0, sizeof(text_buf));
    memcpy(text_buf, source_text, (size_t)source_text_len);

    cre2_options_t * opt = NULL;
    cre2_set * set = NULL;
    cre2_regexp_t * re = NULL;

    int add_res1 = 0;
    int add_res2 = 0;
    cre2_encoding_t enc = CRE2_UNKNOWN;
    const char * errstr = NULL;
    long checksum = 0;

    // Step 2: Setup
    opt = cre2_opt_new();
    set = cre2_set_new(opt, CRE2_UNANCHORED);
    add_res1 = cre2_set_add_simple(set, "a+");
    add_res2 = cre2_set_add_simple(set, "b?");
    cre2_set_compile(set);
    re = cre2_new(pattern, pattern_len, opt);

    // Step 3: Core operations
    enc = cre2_opt_encoding(opt);
    errstr = cre2_error_string(re);
    add_res2 = cre2_set_add_simple(set, "foo"); /* additional add to exercise API */

    // Step 4: Validate
    checksum = (long)pattern_len + (long)source_text_len + (long)enc + (long)add_res1 + (long)add_res2 + (long)(errstr != NULL) + (long)(re != NULL) + (long)(set != NULL) + (long)(opt != NULL) + (long)text_buf[0];
    (void)checksum;

    // Step 5: Cleanup
    cre2_set_delete(set);
    cre2_delete(re);
    cre2_opt_delete(opt);

    // step 6: API sequence test completed successfully
    return 66;
}