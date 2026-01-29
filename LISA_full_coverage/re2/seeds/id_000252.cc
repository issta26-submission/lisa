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
//<ID> 252
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(foo)-(\\d+)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char text_src[] = "foo-1234 trailing";
    const int text_src_len = (int)(sizeof(text_src) - 1);
    char input_buf[64];
    char match_buf[64];
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_string_t input_s;
    cre2_string_t match_s;
    int match_res = 0;
    int ver_rev = 0;
    const int nmatch = 2;

    // step 2: Setup
    memset(input_buf, 0, sizeof(input_buf));
    memset(match_buf, 0, sizeof(match_buf));
    memset(&input_s, 0, sizeof(input_s));
    memset(&match_s, 0, sizeof(match_s));
    memcpy(input_buf, text_src, (size_t)text_src_len);
    input_s.data = input_buf;
    input_s.length = text_src_len;
    match_s.data = match_buf;
    match_s.length = 0;
    opt = cre2_opt_new();
    cre2_opt_set_perl_classes(opt, 1);
    cre2_opt_set_never_nl(opt, 1);

    // step 3: Create / Configure regex
    re = cre2_new(pattern, pattern_len, opt);

    // step 4: Core operations
    match_res = cre2_easy_match(pattern, pattern_len, input_s.data, input_s.length, &match_s, nmatch);
    ver_rev = cre2_version_interface_revision();

    // step 5: Inspect / propagate results
    match_s.length = match_s.length + ((match_res & 0x7) + (ver_rev & 0x3));
    input_s.length = input_s.length + ((int)match_s.length & 0xF);
    input_buf[0] = (char)((unsigned char)input_buf[0] + ((unsigned)match_s.length & 0x7));
    match_buf[0] = (char)((unsigned char)match_buf[0] + ((unsigned)input_s.length & 0x7));
    (void)match_res;
    (void)ver_rev;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);
    // API sequence test completed successfully
    return 66;
}