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
//<ID> 223
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
    char text_buf[256];
    char match_buf[256];
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_string_t text_s;
    cre2_string_t match_s;
    const char * ver = NULL;
    int res_find = 0;
    int ver0 = 0;

    // step 2: Setup
    memset(text_buf, 0, sizeof(text_buf));
    memset(match_buf, 0, sizeof(match_buf));
    memset(&text_s, 0, sizeof(text_s));
    memset(&match_s, 0, sizeof(match_s));
    memcpy(text_buf, input_text, (size_t)input_text_len);
    text_s.data = text_buf;
    text_s.length = input_text_len;
    match_s.data = match_buf;
    match_s.length = 0;
    opt = cre2_opt_new();
    cre2_opt_set_case_sensitive(opt, 1);
    cre2_opt_set_perl_classes(opt, 1);

    // step 3: Core operations
    re = cre2_new(pattern, pattern_len, opt);
    res_find = cre2_find_and_consume(pattern, &text_s, &match_s, 2);
    ver = cre2_version_string();
    ver0 = (int)((unsigned char)ver[0]);

    // step 4: Inspect results
    match_s.length = match_s.length + ((res_find + ver0) & 0xF);
    text_s.length = text_s.length + ((int)match_s.length & 0xF);

    // step 5: Validate / propagate results
    text_buf[0] = (char)((unsigned char)text_buf[0] + ((unsigned)text_s.length & 0x7));
    match_buf[0] = (char)((unsigned char)match_buf[0] + ((unsigned)match_s.length & 0x7));
    (void)ver0;
    (void)res_find;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}