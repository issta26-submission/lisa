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
//<ID> 1986
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern1[] = "foo|bar";
    const size_t pattern1_len = (size_t)(sizeof(pattern1) - 1);
    const char pattern2[] = "(hello)";
    const size_t pattern2_len = (size_t)(sizeof(pattern2) - 1);
    const char text[] = "hello and foo and bar";
    const size_t text_len = (size_t)(sizeof(text) - 1);
    cre2_options_t * opt = NULL;
    cre2_set * set = NULL;
    char errbuf[128];
    int matches[8];
    size_t match_count = 0;
    int add_ret1 = 0;
    int add_ret2 = 0;
    long checksum = 0;

    // step 2: Setup
    memset(errbuf, 0, sizeof(errbuf));
    memset(matches, 0, sizeof(matches));
    opt = cre2_opt_new();
    cre2_opt_set_one_line(opt, 1);

    // step 3: Operate - create set and add patterns
    set = cre2_set_new(opt, CRE2_UNANCHORED);
    add_ret1 = cre2_set_add(set, pattern1, pattern1_len, errbuf, sizeof(errbuf));
    add_ret2 = cre2_set_add(set, pattern2, pattern2_len, errbuf, sizeof(errbuf));

    // step 4: Operate - match against text
    match_count = cre2_set_match(set, text, text_len, matches, 8);

    // step 5: Validate
    checksum = (long)pattern1_len + (long)pattern2_len + (long)text_len + (long)match_count + (long)add_ret1 + (long)add_ret2 + (long)matches[0] + (long)matches[1] + (long)(opt != NULL) + (long)(set != NULL);
    (void)checksum;

    // step 6: Cleanup
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}