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
//<ID> 227
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
    const char input_text[] = "prefix foobar suffix";
    const int input_text_len = (int)(sizeof(input_text) - 1);
    char text_buf[128];
    char target_buf[128];
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_string_t text_and_target;
    cre2_string_t matches[4];
    cre2_string_t target_s;
    const char * ver = NULL;
    int res_find = 0;
    const int nmatch = 3;

    // step 2: Setup
    memset(text_buf, 0, sizeof(text_buf));
    memset(target_buf, 0, sizeof(target_buf));
    memset(matches, 0, sizeof(matches));
    memset(&text_and_target, 0, sizeof(text_and_target));
    memset(&target_s, 0, sizeof(target_s));
    memcpy(text_buf, input_text, (size_t)input_text_len);
    text_and_target.data = text_buf;
    text_and_target.length = input_text_len;
    target_s.data = target_buf;
    target_s.length = 0;
    opt = cre2_opt_new();
    cre2_opt_set_case_sensitive(opt, 1);
    cre2_opt_set_perl_classes(opt, 1);

    // step 3: Core operations
    re = cre2_new(pattern, pattern_len, opt);
    ver = cre2_version_string();
    res_find = cre2_find_and_consume(pattern, &text_and_target, &matches[0], nmatch);

    // step 4: Inspect results
    matches[0].length = matches[0].length + ((res_find + text_and_target.length) & 7);
    matches[1].length = matches[1].length + ((int)(matches[0].length) & 3);
    target_s.length = target_s.length + ((matches[1].length ^ (int)(unsigned char)ver[0]) & 0xF);
    text_and_target.length = text_and_target.length + ((target_s.length + res_find) & 3);

    // step 5: Validate / propagate results
    text_buf[0] = (char)((unsigned char)text_buf[0] + ((unsigned)text_and_target.length & 0x7));
    text_buf[1] = (char)((unsigned char)text_buf[1] + ((unsigned)matches[0].length & 0x7));
    target_buf[0] = (char)((unsigned char)target_buf[0] + ((unsigned)target_s.length & 0x7));
    (void)matches;
    (void)pattern_len;
    (void)input_text_len;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}