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
//<ID> 1980
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "foo|bar";
    const size_t pattern_len = (size_t)(sizeof(pattern) - 1);
    const char text[] = "this contains foo and also bar";
    const size_t text_len = (size_t)(sizeof(text) - 1);
    cre2_options_t * opt = NULL;
    cre2_set * set = NULL;
    char errbuf[128];
    int matches[10];
    size_t match_count = 0;
    int add_ret = 0;
    int compile_ret = 0;
    long checksum = 0;

    // step 2: Setup
    memset(errbuf, 0, sizeof(errbuf));
    memset(matches, 0, sizeof(matches));
    opt = cre2_opt_new();
    cre2_opt_set_one_line(opt, 1);

    // step 3: Core operations
    set = cre2_set_new(opt, CRE2_UNANCHORED);
    add_ret = cre2_set_add(set, pattern, pattern_len, errbuf, sizeof(errbuf));
    compile_ret = cre2_set_compile(set);
    match_count = cre2_set_match(set, text, text_len, matches, sizeof(matches) / sizeof(matches[0]));

    // step 4: Validate
    checksum = (long)pattern_len + (long)text_len + (long)add_ret + (long)compile_ret + (long)match_count + (long)matches[0] + (long)matches[1] + (long)(opt != NULL) + (long)(set != NULL) + (long)errbuf[0];
    (void)checksum;

    // step 5: Cleanup
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}