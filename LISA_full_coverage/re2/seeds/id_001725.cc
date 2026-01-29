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
//<ID> 1725
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
    const char source_text[] = "foobar foo baz";
    const int source_text_len = (int)(sizeof(source_text) - 1);
    char text_and_target_buf[64];
    char rewrite_buf[16];
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_named_groups_iter_t * ng_iter = NULL;
    cre2_string_t text_and_target_str;
    cre2_string_t rewrite_str;
    cre2_string_t pmatch[4];
    const char * ng_name = NULL;
    int ng_index = -1;
    int rc_easy = 0;
    int rc_replace = 0;
    long checksum = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    cre2_opt_set_perl_classes(opt, 1);

    // step 3: Initialize buffers and objects
    memset(text_and_target_buf, 0, sizeof(text_and_target_buf));
    memset(rewrite_buf, 0, sizeof(rewrite_buf));
    memcpy(text_and_target_buf, source_text, (size_t)source_text_len);
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
    rc_easy = cre2_easy_match(pattern, pattern_len, source_text, source_text_len, pmatch, 4);
    re = cre2_new(pattern, pattern_len, opt);
    ng_iter = cre2_named_groups_iter_new(re);
    cre2_named_groups_iter_next(ng_iter, &ng_name, &ng_index);
    rc_replace = cre2_replace_re(re, &text_and_target_str, &rewrite_str);

    // step 5: Validate (compute checksum to use results)
    checksum = (long)pattern_len + (long)source_text_len + (long)rc_easy + (long)rc_replace + (long)ng_index + (long)pmatch[0].length + (long)pmatch[1].length + (long)pmatch[2].length + (long)text_and_target_str.length + (long)rewrite_str.length + (long)(re != NULL) + (long)(opt != NULL);
    (void)checksum;

    // step 6: Cleanup
    cre2_named_groups_iter_delete(ng_iter);
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}