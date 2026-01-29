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
//<ID> 1718
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(hello)(world)?";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char set_pattern[] = "hell.*";
    const char subject[] = "helloworld! hello world!";
    const int subject_len = (int)(sizeof(subject) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_set * set_ptr = NULL;
    cre2_string_t pmatch[3];
    int match_ret = 0;
    int prog_size = 0;
    int set_matches[4];
    size_t set_result = 0;
    long checksum = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    re = cre2_new(pattern, pattern_len, opt);
    set_ptr = cre2_set_new(opt, CRE2_UNANCHORED);
    cre2_set_add_simple(set_ptr, set_pattern);

    // step 3: Configure
    cre2_set_compile(set_ptr);

    // step 4: Operate
    memset(pmatch, 0, sizeof(pmatch));
    memset(set_matches, 0, sizeof(set_matches));
    match_ret = cre2_match(re, subject, subject_len, 0, subject_len, CRE2_UNANCHORED, pmatch, 3);
    prog_size = cre2_program_size(re);
    set_result = cre2_set_match(set_ptr, subject, (size_t)subject_len, set_matches, (size_t)4);

    // step 5: Validate
    checksum = (long)pattern_len + (long)subject_len + (long)match_ret + (long)prog_size + (long)set_result + (long)(pmatch[0].length) + (long)(set_matches[0]);
    (void)checksum;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_set_delete(set_ptr);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}