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
//<ID> 308
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    cre2_options_t * opt = NULL;
    int initial_one_line = 0;
    int one_line_after_set = 0;
    int computed_word_boundary = 0;
    int perl_classes_flag = 1;
    int checksum = 0;
    char buffer[32];
    memset(buffer, 0, sizeof(buffer));

    // step 2: Setup
    opt = cre2_opt_new();

    // step 3: Configure
    initial_one_line = cre2_opt_one_line(opt);
    computed_word_boundary = (initial_one_line & 1);
    cre2_opt_set_word_boundary(opt, computed_word_boundary);
    cre2_opt_set_perl_classes(opt, perl_classes_flag);

    // step 4: Operate / Validate
    one_line_after_set = cre2_opt_one_line(opt);
    checksum = initial_one_line + one_line_after_set + computed_word_boundary + perl_classes_flag;
    buffer[0] = (char)(checksum & 0xFF);

    // step 5: Cleanup
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}