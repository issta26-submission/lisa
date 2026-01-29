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
//<ID> 229
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "([A-Za-z]+)-(\\d+)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char input_text[] = "item-123 extra";
    const int input_text_len = (int)(sizeof(input_text) - 1);
    char text_buf[128];
    char target_buf[128];
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_string_t text_and_target;
    cre2_string_t match_s;
    const char * ver = NULL;
    const char * compiled_pat = NULL;
    int res_find = 0;
    const int nmatch = 2;

    // step 2: Setup
    memset(text_buf, 0, sizeof(text_buf));
    memset(target_buf, 0, sizeof(target_buf));
    memset(&text_and_target, 0, sizeof(text_and_target));
    memset(&match_s, 0, sizeof(match_s));
    opt = cre2_opt_new();
    cre2_opt_set_case_sensitive(opt, 1);
    cre2_opt_set_perl_classes(opt, 1);
    re = cre2_new(pattern, pattern_len, opt);
    memcpy(text_buf, input_text, (size_t)input_text_len);
    text_and_target.data = text_buf;
    text_and_target.length = input_text_len;
    match_s.data = target_buf;
    match_s.length = 0;

    // step 3: Core operations
    res_find = cre2_find_and_consume(pattern, &text_and_target, &match_s, nmatch);
    ver = cre2_version_string();
    compiled_pat = cre2_pattern(re);

    // step 4: Inspect results
    match_s.length = match_s.length + ((res_find & 7) + ((ver != NULL) ? ((int)ver[0] & 3) : 0));
    text_and_target.length = text_and_target.length + ((match_s.length ^ (int)(compiled_pat != NULL ? compiled_pat[0] : 0)) & 0xF);

    // step 5: Validate / propagate results
    text_buf[0] = (char)((unsigned char)text_buf[0] + ((unsigned)text_and_target.length & 0x7));
    target_buf[0] = (char)((unsigned char)target_buf[0] + ((unsigned)match_s.length & 0x7));
    (void)ver;
    (void)compiled_pat;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}