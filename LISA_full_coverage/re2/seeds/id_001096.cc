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
//<ID> 1096
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "^(user)([0-9]{1,3})?(?:-tag)?";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    char text_buf[] = "user42-tag extra";
    const int text_len = (int)(sizeof(text_buf) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_set * set = NULL;
    cre2_string_t matches[8];
    const int nmatch = (int)(sizeof(matches) / sizeof(matches[0]));
    cre2_string_t text_str;
    int partial_ret = 0;
    int compile_ret = 0;
    int groups = 0;
    long checksum = 0;

    // step 2: Setup (options and set)
    opt = cre2_opt_new();
    cre2_opt_set_case_sensitive(opt, 1);
    cre2_opt_set_perl_classes(opt, 1);
    set = cre2_set_new(opt, CRE2_UNANCHORED);
    cre2_set_add_simple(set, pattern);

    // step 3: Core operations (compile set and create regexp)
    compile_ret = cre2_set_compile(set);
    re = cre2_new(pattern, pattern_len, opt);

    // step 4: Operate (prepare strings and perform a partial match)
    memset(matches, 0, sizeof(matches));
    text_str.data = text_buf;
    text_str.length = text_len;
    partial_ret = cre2_partial_match(pattern, &text_str, matches, nmatch);
    groups = cre2_num_capturing_groups(re);

    // step 5: Validate / compute lightweight checksum
    checksum = (long)pattern_len + (long)text_len + (long)compile_ret + (long)partial_ret + (long)groups + (long)(matches[0].length);
    (void)checksum;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}