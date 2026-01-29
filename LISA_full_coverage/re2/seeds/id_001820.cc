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
//<ID> 1820
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(foo)(bar)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char source_text[] = "xx foobar yy";
    const int source_text_len = (int)(sizeof(source_text) - 1);
    char text_buf[128];
    char rewrite_buf[64];
    char errmsg_buf[128];
    memset(text_buf, 0, sizeof(text_buf));
    memset(rewrite_buf, 0, sizeof(rewrite_buf));
    memset(errmsg_buf, 0, sizeof(errmsg_buf));
    cre2_options_t *opt = NULL;
    cre2_regexp_t *re = NULL;
    cre2_string_t text;
    cre2_string_t matches[3];
    cre2_string_t single_match;
    cre2_string_t rewrite;
    cre2_string_t errmsg;
    int num_groups = 0;
    int easy_res = 0;
    int find_consume_res = 0;
    int check_rewrite_res = 0;
    long checksum = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    cre2_opt_set_case_sensitive(opt, 1);
    cre2_opt_set_perl_classes(opt, 1);
    memcpy(text_buf, source_text, (size_t)source_text_len);
    /* prepare a simple rewrite that refers to first capture group */
    const char rewrite_literal[] = "$1X";
    memcpy(rewrite_buf, rewrite_literal, (size_t)(sizeof(rewrite_literal) - 1));
    text.data = text_buf;
    text.length = source_text_len;
    matches[0].data = text_buf; matches[0].length = 0;
    matches[1].data = text_buf; matches[1].length = 0;
    matches[2].data = text_buf; matches[2].length = 0;
    single_match.data = text_buf; single_match.length = 0;
    rewrite.data = rewrite_buf; rewrite.length = (int)(sizeof(rewrite_literal) - 1);
    errmsg.data = errmsg_buf; errmsg.length = 0;

    // step 3: Configure
    re = cre2_new(pattern, pattern_len, opt);

    // step 4: Operate
    num_groups = cre2_num_capturing_groups(re);
    easy_res = cre2_easy_match(pattern, pattern_len, text_buf, source_text_len, matches, 3);
    find_consume_res = cre2_find_and_consume("foo", &text, &single_match, 1);
    check_rewrite_res = cre2_check_rewrite_string(re, &rewrite, &errmsg);

    // step 5: Validate
    checksum = (long)pattern_len + (long)text.length + (long)num_groups + (long)easy_res + (long)find_consume_res + (long)check_rewrite_res + (long)(re != NULL) + (long)(opt != NULL) + (long)matches[0].length + (long)single_match.length + (long)errmsg.length;
    (void)checksum;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}