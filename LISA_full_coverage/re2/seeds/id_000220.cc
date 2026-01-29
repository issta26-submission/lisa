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
//<ID> 220
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
    char target_buf[128];
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_string_t text_s;
    cre2_string_t target_s;
    const char * ver = NULL;
    int res_find = 0;

    // step 2: Setup
    memset(text_buf, 0, sizeof(text_buf));
    memset(target_buf, 0, sizeof(target_buf));
    memset(&text_s, 0, sizeof(text_s));
    memset(&target_s, 0, sizeof(target_s));
    memcpy(text_buf, original_text, (size_t)original_text_len);
    text_s.data = text_buf;
    text_s.length = original_text_len;
    target_s.data = target_buf;
    target_s.length = 0;
    opt = cre2_opt_new();
    cre2_opt_set_case_sensitive(opt, 1);
    cre2_opt_set_perl_classes(opt, 1);
    re = cre2_new(pattern, pattern_len, opt);
    ver = cre2_version_string();

    // step 3: Core operations
    res_find = cre2_find_and_consume(pattern, &text_s, &target_s, 3);

    // step 4: Inspect results
    target_s.length = target_s.length + ((res_find + (int)(unsigned char)ver[0]) & 7);
    text_s.length = text_s.length + ((target_s.length) & 3);
    text_buf[0] = (char)((unsigned char)text_buf[0] + ((unsigned)text_s.length & 0x7));
    target_buf[0] = (char)((unsigned char)target_buf[0] + ((unsigned)target_s.length & 0x7));

    // step 5: Validate / propagate results
    text_buf[1] = (char)((unsigned char)text_buf[1] + ((unsigned)res_find & 0x7));
    (void)pattern_len;
    (void)original_text_len;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}