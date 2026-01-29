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
//<ID> 1984
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern1[] = "foo";
    const char pattern2[] = "bar|baz";
    const size_t pattern1_len = (size_t)(sizeof(pattern1) - 1);
    const size_t pattern2_len = (size_t)(sizeof(pattern2) - 1);
    const char text[] = "this is foo and baz in text";
    const size_t text_len = (size_t)(sizeof(text) - 1);
    cre2_options_t * opt = NULL;
    cre2_set * set = NULL;
    char errbuf1[128];
    char errbuf2[128];
    int match_buf[8];
    size_t match_count = 0;
    int add_ret1 = 0;
    int add_ret2 = 0;
    int compile_ret = 0;
    long checksum = 0;

    // step 2: Setup
    memset(errbuf1, 0, sizeof(errbuf1));
    memset(errbuf2, 0, sizeof(errbuf2));
    memset(match_buf, 0, sizeof(match_buf));
    opt = cre2_opt_new();
    cre2_opt_set_one_line(opt, 1);
    set = cre2_set_new(opt, CRE2_UNANCHORED);

    // step 3: Operate - add patterns and compile set
    add_ret1 = cre2_set_add(set, pattern1, pattern1_len, errbuf1, sizeof(errbuf1));
    add_ret2 = cre2_set_add(set, pattern2, pattern2_len, errbuf2, sizeof(errbuf2));
    compile_ret = cre2_set_compile(set);

    // step 4: Operate - perform matching against text and gather results
    match_count = cre2_set_match(set, text, text_len, match_buf, (size_t)(sizeof(match_buf)/sizeof(match_buf[0])));

    // step 5: Validate
    checksum = (long)pattern1_len + (long)pattern2_len + (long)text_len + (long)match_count + (long)add_ret1 + (long)add_ret2 + (long)compile_ret + (long)match_buf[0] + (long)match_buf[1] + (long)(opt != NULL) + (long)(set != NULL) + (long)errbuf1[0] + (long)errbuf2[0];
    (void)checksum;

    // step 6: Cleanup
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}