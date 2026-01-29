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
//<ID> 838
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pat1[] = "foo";
    const int pat1_len = (int)(sizeof(pat1) - 1);
    const char pat2[] = "bar";
    const int pat2_len = (int)(sizeof(pat2) - 1);
    char text_buf[] = "xx foobar yy";
    const int text_len = (int)(sizeof(text_buf) - 1);
    cre2_options_t * opt = NULL;
    cre2_set * set = NULL;
    cre2_regexp_t * re = NULL;
    cre2_named_groups_iter_t * iter = NULL;
    cre2_encoding_t enc = CRE2_UNKNOWN;
    int case_flag = 1;
    int match_indices[4];
    size_t matches_found = 0;
    long computed_check = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    cre2_opt_set_case_sensitive(opt, case_flag);
    enc = cre2_opt_encoding(opt);
    set = cre2_set_new(opt, CRE2_UNANCHORED);

    // step 3: Core operations
    cre2_set_add_simple(set, pat1);
    cre2_set_add_simple(set, pat2);
    cre2_set_compile(set);
    memset(match_indices, 0, sizeof(match_indices));
    matches_found = cre2_set_match(set, text_buf, text_len, match_indices, 4);
    re = cre2_new(pat1, pat1_len, opt);
    iter = cre2_named_groups_iter_new(re);

    // step 4: Validate / intermediate cleanup
    cre2_named_groups_iter_delete(iter);
    computed_check = (long)matches_found + (long)enc + (long)case_flag + (long)match_indices[0];
    (void)computed_check;

    // step 5: Cleanup
    cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}