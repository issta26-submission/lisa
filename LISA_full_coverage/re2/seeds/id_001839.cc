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
//<ID> 1839
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
    memcpy(text_buf, source_text, (size_t)source_text_len);
    memcpy(consume_buf, source_text, (size_t)source_text_len);

    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;

    cre2_string_t easy_matches[4];
    cre2_string_t partial_matches[4];
    cre2_string_t consume_matches[4];
    memset(easy_matches, 0, sizeof(easy_matches));
    memset(partial_matches, 0, sizeof(partial_matches));
    memset(consume_matches, 0, sizeof(consume_matches));

    cre2_string_t text_input_const;
    cre2_string_t text_input_mut;
    text_input_const.data = text_buf;
    text_input_const.length = source_text_len;
    text_input_mut.data = consume_buf;
    text_input_mut.length = source_text_len;

    int perl_flag = 0;
    int case_flag = 1;
    int easy_res = 0;
    int partial_res = 0;
    int consume_res = 0;
    long checksum = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    cre2_opt_set_perl_classes(opt, 1);
    perl_flag = cre2_opt_perl_classes(opt);
    cre2_opt_set_case_sensitive(opt, case_flag);
    re = cre2_new(pattern, pattern_len, opt);

    // step 3: Core operations
    easy_matches[0].data = text_buf;
    easy_matches[0].length = 0;
    easy_matches[1].data = text_buf;
    easy_matches[1].length = 0;
    easy_matches[2].data = text_buf;
    easy_matches[2].length = 0;
    easy_res = cre2_easy_match(pattern, pattern_len, text_buf, source_text_len, easy_matches, 3);

    partial_matches[0].data = text_buf;
    partial_matches[0].length = 0;
    partial_matches[1].data = text_buf;
    partial_matches[1].length = 0;
    partial_matches[2].data = text_buf;
    partial_matches[2].length = 0;
    partial_res = cre2_partial_match_re(re, &text_input_const, partial_matches, 3);

    consume_matches[0].data = consume_buf;
    consume_matches[0].length = 0;
    consume_matches[1].data = consume_buf;
    consume_matches[1].length = 0;
    consume_matches[2].data = consume_buf;
    consume_matches[2].length = 0;
    consume_res = cre2_consume_re(re, &text_input_mut, consume_matches, 3);

    // step 4: Validate
    checksum = (long)pattern_len + (long)source_text_len + (long)perl_flag + (long)case_flag + (long)(re != NULL) + (long)easy_res + (long)partial_res + (long)consume_res + (long)easy_matches[0].length + (long)partial_matches[0].length + (long)consume_matches[0].length;
    (void)checksum;

    // step 5: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}