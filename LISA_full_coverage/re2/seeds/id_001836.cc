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
//<ID> 1836
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(ab)(cd)?";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char source_text[] = "xx abcd yy";
    const int source_text_len = (int)(sizeof(source_text) - 1);
    char text_buf[128];
    memset(text_buf, 0, sizeof(text_buf));
    cre2_options_t *opt = NULL;
    cre2_regexp_t *re = NULL;
    cre2_string_t input;
    cre2_string_t matches[4];
    cre2_string_t easy_matches[4];
    cre2_string_t consumed;
    memset(matches, 0, sizeof(matches));
    memset(easy_matches, 0, sizeof(easy_matches));
    int opt_perl = 0;
    int opt_case_flag = 1;
    int easy_res = 0;
    int partial_res = 0;
    int consume_res = 0;
    long checksum = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    cre2_opt_set_case_sensitive(opt, opt_case_flag);
    opt_perl = cre2_opt_perl_classes(opt);
    memcpy(text_buf, source_text, (size_t)source_text_len);
    input.data = text_buf;
    input.length = source_text_len;
    consumed.data = text_buf;
    consumed.length = 0;
    matches[0].data = text_buf;
    matches[0].length = 0;
    matches[1].data = text_buf;
    matches[1].length = 0;
    matches[2].data = text_buf;
    matches[2].length = 0;
    matches[3].data = text_buf;
    matches[3].length = 0;
    easy_matches[0].data = text_buf;
    easy_matches[0].length = 0;
    easy_matches[1].data = text_buf;
    easy_matches[1].length = 0;
    easy_matches[2].data = text_buf;
    easy_matches[2].length = 0;
    easy_matches[3].data = text_buf;
    easy_matches[3].length = 0;

    // step 3: Configure
    re = cre2_new(pattern, pattern_len, opt);

    // step 4: Operate
    easy_res = cre2_easy_match(pattern, pattern_len, text_buf, source_text_len, easy_matches, 3);
    partial_res = cre2_partial_match_re(re, &input, matches, 3);
    consume_res = cre2_consume_re(re, &input, &consumed, 2);

    // step 5: Validate
    checksum = 0;
    checksum += (long)pattern_len;
    checksum += (long)source_text_len;
    checksum += (long)opt_perl;
    checksum += (long)opt_case_flag;
    checksum += (long)easy_res;
    checksum += (long)partial_res;
    checksum += (long)consume_res;
    checksum += (long)(re != NULL);
    checksum += (long)(opt != NULL);
    checksum += (long)matches[0].length;
    checksum += (long)matches[1].length;
    checksum += (long)easy_matches[0].length;
    checksum += (long)consumed.length;
    (void)checksum;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}