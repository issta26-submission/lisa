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
//<ID> 1834
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
    char text_buf[256];
    memset(text_buf, 0, sizeof(text_buf));
    cre2_options_t *opt = NULL;
    cre2_regexp_t *re = NULL;
    cre2_string_t matches[4];
    memset(matches, 0, sizeof(matches));
    cre2_string_t text;
    text.data = NULL;
    text.length = 0;
    int easy_res = 0;
    int partial_res = 0;
    int consume_res = 0;
    int perl_flag = 0;
    long checksum = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    cre2_opt_set_case_sensitive(opt, 1);
    perl_flag = cre2_opt_perl_classes(opt);
    memcpy(text_buf, source_text, (size_t)source_text_len);
    text.data = text_buf;
    text.length = source_text_len;
    matches[0].data = text_buf;
    matches[0].length = 0;
    matches[1].data = text_buf;
    matches[1].length = 0;
    matches[2].data = text_buf;
    matches[2].length = 0;
    matches[3].data = text_buf;
    matches[3].length = 0;

    // step 3: Configure
    re = cre2_new(pattern, pattern_len, opt);

    // step 4: Operate
    easy_res = cre2_easy_match(pattern, pattern_len, text_buf, source_text_len, matches, 3);
    partial_res = cre2_partial_match_re(re, &text, matches, 4);
    consume_res = cre2_consume_re(re, &text, matches, 4);

    // step 5: Validate
    checksum = (long)pattern_len + (long)text.length + (long)easy_res + (long)partial_res + (long)consume_res + (long)(re != NULL) + (long)(opt != NULL) + (long)perl_flag + (long)matches[0].length;
    (void)checksum;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}