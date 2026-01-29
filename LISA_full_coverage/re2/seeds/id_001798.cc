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
//<ID> 1798
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "foo(bar)?";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char source_text[] = "xx foobar yy";
    const int source_text_len = (int)(sizeof(source_text) - 1);
    char work_buf[64];
    memset(work_buf, 0, sizeof(work_buf));
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_set * set = NULL;
    cre2_string_t match[4];
    memset(match, 0, sizeof(match));
    cre2_encoding_t enc = CRE2_UNKNOWN;
    int compile_res = 0;
    int match_res = 0;
    long checksum = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    cre2_opt_set_perl_classes(opt, 1);
    cre2_opt_set_posix_syntax(opt, 0);
    enc = cre2_opt_encoding(opt);
    re = cre2_new(pattern, pattern_len, opt);

    // step 3: Configure
    set = cre2_set_new(opt, (cre2_anchor_t)CRE2_UNANCHORED);
    cre2_set_add_simple(set, pattern);
    compile_res = cre2_set_compile(set);
    memcpy(work_buf, source_text, (size_t)source_text_len);

    // step 4: Operate
    match[0].data = NULL;
    match[0].length = 0;
    match[1].data = NULL;
    match[1].length = 0;
    match[2].data = NULL;
    match[2].length = 0;
    match[3].data = NULL;
    match[3].length = 0;
    match_res = cre2_match(re, work_buf, source_text_len, 0, source_text_len, (cre2_anchor_t)CRE2_UNANCHORED, match, 4);

    // step 5: Validate
    checksum = (long)pattern_len + (long)source_text_len + (long)enc + (long)compile_res + (long)match_res + (long)(match[0].length) + (long)(re != NULL) + (long)(opt != NULL) + (long)(set != NULL);
    (void)checksum;

    // step 6: Cleanup
    cre2_set_delete(set);
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}