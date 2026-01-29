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
//<ID> 588
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "^(foo|bar)+$";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char text_buf[] = "xxfoobarxxfoobaryy";
    const int text_len = (int)(sizeof(text_buf) - 1);
    cre2_options_t *opt = NULL;
    cre2_regexp_t *re = NULL;
    cre2_set *set = NULL;
    cre2_encoding_t enc = CRE2_UNKNOWN;
    int add_rc = 0;
    int compile_rc = 0;
    size_t setmatch_count = 0;
    int match_ints[4];
    size_t match_len = 4;
    int errcode = 0;
    int computed_check = 0;

    // step 2: Setup
    memset(match_ints, 0, sizeof(match_ints));
    opt = cre2_opt_new();

    // step 3: Configure
    cre2_opt_set_encoding(opt, CRE2_UTF8);
    enc = cre2_opt_encoding(opt);

    // step 4: Operate
    re = cre2_new(pattern, pattern_len, opt);
    set = cre2_set_new(opt, (cre2_anchor_t)0);
    add_rc = cre2_set_add_simple(set, pattern);
    compile_rc = cre2_set_compile(set);
    setmatch_count = cre2_set_match(set, text_buf, (size_t)text_len, match_ints, match_len);
    errcode = cre2_error_code(re);

    // step 5: Validate
    computed_check = add_rc + compile_rc + (int)setmatch_count + errcode + (int)enc + match_ints[0] + match_ints[1];
    (void)computed_check;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}