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
//<ID> 1824
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
    const char easy_pattern[] = "foo";
    const int easy_pattern_len = (int)(sizeof(easy_pattern) - 1);
    const char source_text[] = "xx foobar yy";
    const int source_text_len = (int)(sizeof(source_text) - 1);
    char work_buf[256];
    char rewrite_buf[64];
    char errmsg_buf[128];
    memset(work_buf, 0, sizeof(work_buf));
    memset(rewrite_buf, 0, sizeof(rewrite_buf));
    memset(errmsg_buf, 0, sizeof(errmsg_buf));
    cre2_options_t * opt = NULL;
    cre2_regexp_t * rex = NULL;
    cre2_string_t text_and_target;
    cre2_string_t rewrite;
    cre2_string_t errmsg;
    cre2_string_t easy_matches[2];
    cre2_string_t find_match;
    int num_groups = 0;
    int check_res = 0;
    int easy_res = 0;
    int find_res = 0;
    long checksum = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    cre2_opt_set_perl_classes(opt, 1);
    memcpy(work_buf, source_text, (size_t)source_text_len);
    const char rewrite_literal[] = "$1Z";
    memcpy(rewrite_buf, rewrite_literal, (size_t)(sizeof(rewrite_literal) - 1));
    text_and_target.data = work_buf;
    text_and_target.length = source_text_len;
    rewrite.data = rewrite_buf;
    rewrite.length = (int)(sizeof(rewrite_literal) - 1);
    errmsg.data = errmsg_buf;
    errmsg.length = 0;
    easy_matches[0].data = work_buf;
    easy_matches[0].length = 0;
    easy_matches[1].data = work_buf;
    easy_matches[1].length = 0;
    find_match.data = work_buf;
    find_match.length = 0;

    // step 3: Configure
    rex = cre2_new(pattern, pattern_len, opt);
    num_groups = cre2_num_capturing_groups(rex);

    // step 4: Operate
    check_res = cre2_check_rewrite_string(rex, &rewrite, &errmsg);
    easy_res = cre2_easy_match(easy_pattern, easy_pattern_len, work_buf, source_text_len, easy_matches, 2);
    find_res = cre2_find_and_consume(easy_pattern, &text_and_target, &find_match, 1);

    // step 5: Validate
    checksum = (long)pattern_len + (long)source_text_len + (long)num_groups + (long)check_res + (long)easy_res + (long)find_res + (long)rewrite.length + (long)text_and_target.length + (long)(rex != NULL) + (long)(opt != NULL) + (long)errmsg.length + (long)easy_matches[0].length + (long)find_match.length;
    (void)checksum;

    // step 6: Cleanup
    cre2_delete(rex);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}