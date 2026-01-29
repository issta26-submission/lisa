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
//<ID> 1989
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern1[] = "foo([0-9]+)";
    const int pattern1_len = (int)(sizeof(pattern1) - 1);
    const char pattern2[] = "bar(baz)?";
    const int pattern2_len = (int)(sizeof(pattern2) - 1);
    const char text[] = "prefix foo123 middle barbaz suffix";
    const int text_len = (int)(sizeof(text) - 1);
    cre2_options_t * opt = NULL;
    cre2_set * set = NULL;
    char err1[128];
    char err2[128];
    int add_ret1 = 0;
    int add_ret2 = 0;
    int compile_ret = 0;
    int matches[8];
    size_t match_count = 0;
    long checksum = 0;

    // step 2: Setup
    memset(err1, 0, sizeof(err1));
    memset(err2, 0, sizeof(err2));
    memset(matches, 0, sizeof(matches));
    opt = cre2_opt_new();
    cre2_opt_set_one_line(opt, 1);

    // step 3: Core operations
    set = cre2_set_new(opt, CRE2_UNANCHORED);
    add_ret1 = cre2_set_add(set, pattern1, (size_t)pattern1_len, err1, sizeof(err1));
    add_ret2 = cre2_set_add(set, pattern2, (size_t)pattern2_len, err2, sizeof(err2));
    compile_ret = cre2_set_compile(set);
    match_count = cre2_set_match(set, text, (size_t)text_len, matches, (size_t)8);

    // step 4: Validate
    checksum = (long)pattern1_len + (long)pattern2_len + (long)text_len + (long)add_ret1 + (long)add_ret2 + (long)compile_ret + (long)match_count + (long)matches[0] + (long)(opt != NULL) + (long)(set != NULL);
    (void)checksum;

    // step 5: Cleanup
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}