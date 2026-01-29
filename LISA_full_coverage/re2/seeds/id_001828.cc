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
//<ID> 1828
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(foo)(bar)?";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char source_text[] = "xx foobar yy";
    const int source_text_len = (int)(sizeof(source_text) - 1);
    const char rewrite_literal[] = "$1-$2";
    char work_buf[128];
    char rewrite_buf[64];
    char errmsg_buf[128];
    memset(work_buf, 0, sizeof(work_buf));
    memset(rewrite_buf, 0, sizeof(rewrite_buf));
    memset(errmsg_buf, 0, sizeof(errmsg_buf));
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_string_t matches[3];
    cre2_string_t text;
    cre2_string_t rewrite;
    cre2_string_t errmsg;
    matches[0].data = NULL;
    matches[0].length = 0;
    matches[1].data = NULL;
    matches[1].length = 0;
    matches[2].data = NULL;
    matches[2].length = 0;
    int easy_match_res = 0;
    int find_consume_res = 0;
    int check_rewrite_res = 0;
    int num_groups = 0;
    long checksum = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    cre2_opt_set_perl_classes(opt, 1);
    memcpy(work_buf, source_text, (size_t)source_text_len);
    memcpy(rewrite_buf, rewrite_literal, (size_t)(sizeof(rewrite_literal) - 1));
    text.data = work_buf;
    text.length = source_text_len;
    rewrite.data = rewrite_buf;
    rewrite.length = (int)(sizeof(rewrite_literal) - 1);
    errmsg.data = errmsg_buf;
    errmsg.length = (int)(sizeof(errmsg_buf));

    // step 3: Configure
    re = cre2_new(pattern, pattern_len, opt);
    num_groups = cre2_num_capturing_groups(re);
    matches[0].data = work_buf;
    matches[0].length = 0;
    matches[1].data = work_buf;
    matches[1].length = 0;
    matches[2].data = work_buf;
    matches[2].length = 0;

    // step 4: Operate
    check_rewrite_res = cre2_check_rewrite_string(re, &rewrite, &errmsg);
    easy_match_res = cre2_easy_match(pattern, pattern_len, work_buf, source_text_len, matches, 3);
    find_consume_res = cre2_find_and_consume(pattern, &text, matches, 3);

    // step 5: Validate
    checksum = (long)pattern_len + (long)source_text_len + (long)num_groups + (long)check_rewrite_res + (long)easy_match_res + (long)find_consume_res + (long)(re != NULL) + (long)(opt != NULL) + (long)matches[0].length + (long)rewrite.length + (long)errmsg.length;
    (void)checksum;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}