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
//<ID> 1521
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(?P<name>foo)bar";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    cre2_options_t *opt = NULL;
    cre2_regexp_t *re = NULL;
    cre2_named_groups_iter_t *iter = NULL;
    cre2_string_t inputs[1];
    cre2_string_t outputs[1];
    char textbuf[] = "foobar extra";
    const char ng_name[] = "name";
    const char *found_name = NULL;
    int iter_index = -1;
    int partial_ret = 0;
    int find_ret = 0;
    bool iter_next_ret = false;
    long checksum = 0;

    // step 2: Setup
    memset(inputs, 0, sizeof(inputs));
    memset(outputs, 0, sizeof(outputs));
    inputs[0].data = textbuf;
    inputs[0].length = (int)(sizeof(textbuf) - 1);
    opt = cre2_opt_new();

    // step 3: Configure
    cre2_opt_set_log_errors(opt, 1);

    // step 4: Core operations
    re = cre2_new(pattern, pattern_len, opt);
    partial_ret = cre2_partial_match(pattern, inputs, outputs, 1);
    iter = cre2_named_groups_iter_new(re);
    iter_next_ret = cre2_named_groups_iter_next(iter, &found_name, &iter_index);
    find_ret = cre2_find_named_capturing_groups(re, ng_name);

    // step 5: Validate
    checksum = (long)pattern_len + (long)partial_ret + (long)find_ret + (long)(re != NULL)
               + (long)outputs[0].length + (long)iter_index + (long)(found_name != NULL) + (long)iter_next_ret;
    (void)checksum;

    // step 6: Cleanup
    cre2_named_groups_iter_delete(iter);
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}