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
//<ID> 78
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
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_string_t subject;
    cre2_string_t matches[4];
    int perl_flag = 0;
    int num_groups = 0;
    int nmatch = 0;
    int easy_res = 0;
    int partial_res = 0;

    // step 2: Setup
    memset(&subject, 0, sizeof(subject));
    memset(matches, 0, sizeof(matches));
    subject.data = text;
    subject.length = text_len;
    opt = cre2_opt_new();

    // step 3: Configure
    cre2_opt_set_literal(opt, 1);
    perl_flag = cre2_opt_perl_classes(opt);

    // step 4: Operate
    re = cre2_new(pattern, pattern_len, opt);
    num_groups = cre2_num_capturing_groups(re);
    nmatch = num_groups + 1;
    easy_res = cre2_easy_match(pattern, pattern_len, text, text_len, matches, nmatch);
    partial_res = cre2_partial_match(pattern, &subject, matches, nmatch);

    // step 5: Validate
    (void)perl_flag;
    (void)num_groups;
    (void)nmatch;
    (void)easy_res;
    (void)partial_res;
    (void)matches;
    (void)subject;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}