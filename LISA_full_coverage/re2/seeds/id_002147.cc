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
//<ID> 2147
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "([A-Za-z]+)-(\\d+)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    char text1_buf[] = "User-007";
    int text1_len = (int)(sizeof(text1_buf) - 1);
    char text2_buf[] = "PrefixUser-42Suffix";
    int text2_len = (int)(sizeof(text2_buf) - 1);
    char match1_buf[64];
    char match2_buf[64];
    memset(match1_buf, 0, sizeof(match1_buf));
    memset(match2_buf, 0, sizeof(match2_buf));
    cre2_string_t match1 = { match1_buf, 0 };
    cre2_string_t match2 = { match2_buf, 0 };
    cre2_string_t subject2 = { text2_buf, text2_len };
    cre2_options_t * opt = NULL;
    int easy_ret = 0;
    int partial_ret = 0;
    int literal_flag = 0;
    int case_sensitive_flag = 0;
    long checksum = 0;

    // step 2: Setup (create and configure options)
    opt = cre2_opt_new();
    cre2_opt_set_word_boundary(opt, 1);
    cre2_opt_set_case_sensitive(opt, 1);
    literal_flag = cre2_opt_literal(opt);
    case_sensitive_flag = cre2_opt_case_sensitive(opt);

    // step 3: Core operations (perform easy match and partial match)
    easy_ret = cre2_easy_match(pattern, pattern_len, text1_buf, text1_len, &match1, 2);
    partial_ret = cre2_partial_match(pattern, &subject2, &match2, 2);

    // step 4: Validate (combine values so results flow through)
    checksum = (long)pattern_len + (long)easy_ret + (long)partial_ret + (long)match1.length + (long)match2.length + (long)literal_flag + (long)case_sensitive_flag + (long)(opt != NULL);
    (void)checksum;

    // step 5: Cleanup (release resources)
    cre2_opt_delete(opt);

    // step 6: API sequence test completed successfully
    return 66;
}