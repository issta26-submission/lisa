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
//<ID> 1885
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(alpha|beta)(\\d+)?";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char subject[] = "xxx alpha123 yyy";
    const int subject_len = (int)(sizeof(subject) - 1);
    char text_buf[64];
    memset(text_buf, 0, sizeof(text_buf));
    memcpy(text_buf, subject, (size_t)subject_len);

    cre2_options_t * opt = NULL;
    cre2_set * set = NULL;
    cre2_regexp_t * re = NULL;
    cre2_encoding_t enc = CRE2_UNKNOWN;

    int add_res1 = 0;
    int add_res2 = 0;
    size_t found_count = 0;
    int match_indices[8];
    memset(match_indices, 0, sizeof(match_indices));
    const size_t match_len = sizeof(match_indices) / sizeof(match_indices[0]);

    long checksum = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    cre2_opt_set_case_sensitive(opt, 1);
    cre2_opt_set_one_line(opt, 1);
    enc = cre2_opt_encoding(opt);
    set = cre2_set_new(opt, CRE2_UNANCHORED);
    add_res1 = cre2_set_add_simple(set, "alpha");
    add_res2 = cre2_set_add_simple(set, "beta");
    cre2_set_compile(set);
    re = cre2_new(pattern, pattern_len, opt);

    // step 3: Core operations
    found_count = cre2_set_match(set, text_buf, (size_t)subject_len, match_indices, match_len);

    // step 4: Validate
    checksum = (long)pattern_len + (long)subject_len + (long)enc + (long)add_res1 + (long)add_res2 + (long)found_count + (long)(opt != NULL) + (long)(set != NULL) + (long)(re != NULL) + (long)match_indices[0];
    (void)checksum;

    // step 5: Cleanup
    cre2_set_delete(set);
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}