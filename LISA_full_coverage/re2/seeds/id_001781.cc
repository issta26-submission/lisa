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
//<ID> 1781
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(abc)(de)?";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char source_text[] = "xxabcdeYY";
    const int source_text_len = (int)(sizeof(source_text) - 1);
    char work_buf[64];
    memset(work_buf, 0, sizeof(work_buf));
    memcpy(work_buf, source_text, (size_t)source_text_len);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_set * set = NULL;
    cre2_string_t text;
    cre2_string_t caps[4];
    memset(caps, 0, sizeof(caps));
    int partial_res = 0;
    int add_res = 0;
    int compile_res = 0;
    cre2_encoding_t enc = CRE2_UNKNOWN;
    long checksum = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    cre2_opt_set_word_boundary(opt, 1);
    enc = cre2_opt_encoding(opt);
    re = cre2_new(pattern, pattern_len, opt);

    // step 3: Configure
    set = cre2_set_new(opt, (cre2_anchor_t)0);
    add_res = cre2_set_add_simple(set, pattern);
    compile_res = cre2_set_compile(set);
    text.data = work_buf;
    text.length = source_text_len;

    // step 4: Operate
    partial_res = cre2_partial_match(pattern, &text, caps, 3);

    // step 5: Validate
    checksum = (long)pattern_len + (long)source_text_len + (long)enc + (long)partial_res + (long)add_res + (long)compile_res + (long)(re != NULL) + (long)(opt != NULL) + (long)(set != NULL) + (long)caps[0].length;
    (void)checksum;

    // step 6: Cleanup
    cre2_set_delete(set);
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}