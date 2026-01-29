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
//<ID> 1833
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
    char text_buf[128];
    char consume_buf[128];
    memset(text_buf, 0, sizeof(text_buf));
    memset(consume_buf, 0, sizeof(consume_buf));
    cre2_options_t *opt = NULL;
    cre2_regexp_t *re = NULL;
    cre2_string_t text_str;
    cre2_string_t text_and_target;
    cre2_string_t matches[4];
    cre2_string_t consumed_match;
    memset(matches, 0, sizeof(matches));
    int easy_res = 0;
    int partial_res = 0;
    int consume_res = 0;
    int perl_classes_flag = 0;
    long checksum = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    cre2_opt_set_perl_classes(opt, 1);
    perl_classes_flag = cre2_opt_perl_classes(opt);
    cre2_opt_set_case_sensitive(opt, 1);
    memcpy(text_buf, source_text, (size_t)source_text_len);
    memcpy(consume_buf, source_text, (size_t)source_text_len);

    // step 3: Configure
    text_str.data = text_buf;
    text_str.length = source_text_len;
    text_and_target.data = consume_buf;
    text_and_target.length = source_text_len;
    consumed_match.data = consume_buf;
    consumed_match.length = 0;
    matches[0].data = text_buf;
    matches[0].length = 0;
    matches[1].data = text_buf;
    matches[1].length = 0;
    matches[2].data = text_buf;
    matches[2].length = 0;
    matches[3].data = text_buf;
    matches[3].length = 0;
    re = cre2_new(pattern, pattern_len, opt);

    // step 4: Operate
    easy_res = cre2_easy_match(pattern, pattern_len, text_buf, source_text_len, matches, 3);
    partial_res = cre2_partial_match_re(re, &text_str, matches, 3);
    consume_res = cre2_consume_re(re, &text_and_target, &consumed_match, 1);

    // step 5: Validate
    checksum = (long)pattern_len + (long)source_text_len + (long)easy_res + (long)partial_res + (long)consume_res + (long)perl_classes_flag + (long)(re != NULL) + (long)matches[0].length + (long)consumed_match.length + (long)text_and_target.length;
    (void)checksum;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}