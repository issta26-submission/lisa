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
//<ID> 99
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(foo)(bar)?";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char text[] = "prefix foobar suffix";
    const int text_len = (int)(sizeof(text) - 1);
    const char rewrite_template[] = "$1-$2";
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_string_t text_str;
    cre2_string_t rewrite;
    cre2_string_t target;
    cre2_string_t matches[16];
    int perl_flag = 0;
    int n_captures = 0;
    int nm = 0;
    int easy_res = 0;
    int extract_res = 0;

    // step 2: Setup
    memset(&text_str, 0, sizeof(text_str));
    memset(&rewrite, 0, sizeof(rewrite));
    memset(&target, 0, sizeof(target));
    memset(matches, 0, sizeof(matches));
    text_str.data = text;
    text_str.length = text_len;
    rewrite.data = rewrite_template;
    rewrite.length = (int)(sizeof(rewrite_template) - 1);
    opt = cre2_opt_new();

    // step 3: Configure
    cre2_opt_set_perl_classes(opt, 1);
    perl_flag = cre2_opt_perl_classes(opt);

    // step 4: Operate
    re = cre2_new(pattern, pattern_len, opt);
    n_captures = cre2_num_capturing_groups(re);
    nm = n_captures + 1;
    easy_res = cre2_easy_match(pattern, pattern_len, text, text_len, &matches[0], nm);
    extract_res = cre2_extract(pattern, &text_str, &rewrite, &target);

    // step 5: Validate
    (void)perl_flag;
    (void)n_captures;
    (void)nm;
    (void)easy_res;
    (void)extract_res;
    (void)matches;
    (void)text_str;
    (void)rewrite;
    (void)target;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}