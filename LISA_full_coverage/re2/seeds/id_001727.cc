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
//<ID> 1727
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(?P<name>foo)(bar)?";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char source_text[] = "foobar foo";
    const int source_text_len = (int)(sizeof(source_text) - 1);
    char text_and_target_buf[64];
    char rewrite_buf[32];
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_named_groups_iter_t * iter = NULL;
    cre2_string_t text_and_target_str;
    cre2_string_t rewrite_str;
    cre2_string_t pmatch[4];
    const char * found_name = NULL;
    int found_index = -1;
    int easy_res = 0;
    int replace_res = 0;
    long checksum = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    cre2_opt_set_perl_classes(opt, 1);
    re = cre2_new(pattern, pattern_len, opt);

    // step 3: Configure
    memset(text_and_target_buf, 0, sizeof(text_and_target_buf));
    memcpy(text_and_target_buf, source_text, (size_t)source_text_len);
    memset(rewrite_buf, 0, sizeof(rewrite_buf));
    memcpy(rewrite_buf, "$1-$2", 5);

    text_and_target_str.data = text_and_target_buf;
    text_and_target_str.length = source_text_len;
    rewrite_str.data = rewrite_buf;
    rewrite_str.length = 5;

    pmatch[0].data = NULL; pmatch[0].length = 0;
    pmatch[1].data = NULL; pmatch[1].length = 0;
    pmatch[2].data = NULL; pmatch[2].length = 0;
    pmatch[3].data = NULL; pmatch[3].length = 0;

    // step 4: Core operations
    easy_res = cre2_easy_match(pattern, pattern_len, source_text, source_text_len, pmatch, 4);
    iter = cre2_named_groups_iter_new(re);
    cre2_named_groups_iter_next(iter, &found_name, &found_index);
    replace_res = cre2_replace_re(re, &text_and_target_str, &rewrite_str);

    // step 5: Validate
    checksum = (long)pattern_len + (long)source_text_len + (long)easy_res + (long)replace_res + (long)found_index + (long)(found_name != NULL) + (long)text_and_target_str.length + (long)rewrite_str.length + (long)(re != NULL) + (long)(opt != NULL);
    (void)checksum;

    // step 6: Cleanup
    cre2_named_groups_iter_delete(iter);
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}