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
//<ID> 1315
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(?P<word>hello)\\s+world";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    cre2_regexp_t * re = NULL;
    cre2_named_groups_iter_t * iter = NULL;
    const char * re_pat = NULL;
    const char * ng_name = NULL;
    int ng_index = -1;
    int rev = 0;
    char scratch[32];
    long computed_check = 0;

    // step 2: Setup
    memset(scratch, 0, sizeof(scratch));

    // step 3: Core operations
    re = cre2_new(pattern, pattern_len, NULL);

    // step 4: Use iterator and version info
    re_pat = cre2_pattern(re);
    iter = cre2_named_groups_iter_new(re);
    (void)cre2_named_groups_iter_next(iter, &ng_name, &ng_index);
    rev = cre2_version_interface_revision();

    // step 5: Validate / compute lightweight checksum
    computed_check = (long)pattern_len + (long)(re != NULL) + (long)(re_pat != NULL)
                     + (long)(iter != NULL) + (long)ng_index + (long)rev + (long)(scratch[0] == 0);
    (void)computed_check;

    // step 6: Cleanup
    cre2_named_groups_iter_delete(iter);
    cre2_delete(re);

    // API sequence test completed successfully
    return 66;
}