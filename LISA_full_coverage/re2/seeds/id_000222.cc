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
//<ID> 222
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
    const char original_text[] = "prefix foobar suffix";
    const int original_text_len = (int)(sizeof(original_text) - 1);
    char text_buf[128];
    char target_buf[64];
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_string_t text_and_target;
    cre2_string_t target_s;
    const char * ver = NULL;
    int res_find = 0;

    // step 2: Setup
    memset(text_buf, 0, sizeof(text_buf));
    memset(target_buf, 0, sizeof(target_buf));
    memset(&text_and_target, 0, sizeof(text_and_target));
    memset(&target_s, 0, sizeof(target_s));
    opt = cre2_opt_new();
    cre2_opt_set_case_sensitive(opt, 1);
    cre2_opt_set_perl_classes(opt, 1);
    re = cre2_new(pattern, pattern_len, opt);

    // step 3: Core operations
    memcpy(text_buf, original_text, (size_t)original_text_len);
    text_and_target.data = text_buf;
    text_and_target.length = original_text_len;
    target_s.data = target_buf;
    target_s.length = 0;
    res_find = cre2_find_and_consume(pattern, &text_and_target, &target_s, 3);
    ver = cre2_version_string();

    // step 4: Inspect results
    target_s.length = target_s.length + ((res_find + pattern_len) & 7);
    text_and_target.length = text_and_target.length + (target_s.length & 3);

    // step 5: Validate / propagate results
    text_buf[0] = (char)((unsigned char)text_buf[0] + ((unsigned)text_and_target.length + (unsigned)(unsigned char)ver[0] + (unsigned)res_find & 0xF));
    target_buf[0] = (char)((unsigned char)target_buf[0] + ((unsigned)target_s.length & 0x7));
    (void)re;
    (void)pattern_len;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}