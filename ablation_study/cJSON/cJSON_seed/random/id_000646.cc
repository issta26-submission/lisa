#include <cJSON.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <cstring>
#include <fcntl.h>
//<ID> 646
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *str_array = (cJSON *)0;
    cJSON *extra_str = (cJSON *)0;
    cJSON *retrieved = (cJSON *)0;
    cJSON_bool added = 0;
    cJSON_bool is_true = 0;
    int ptr_equal = 0;
    const char *const items[] = { "alpha", "beta", "gamma" };

    // step 2: Initialize
    str_array = cJSON_CreateStringArray(items, 3);
    extra_str = cJSON_CreateString("delta");

    // step 3: Configure
    added = cJSON_AddItemToArray(str_array, extra_str);

    // step 4: Operate
    retrieved = cJSON_GetArrayItem(str_array, 3);
    is_true = cJSON_IsTrue(retrieved);

    // step 5: Validate
    ptr_equal = (retrieved == extra_str);
    (void)added;
    (void)is_true;
    (void)ptr_equal;
    (void)retrieved;

    // step 6: Cleanup
    cJSON_Delete(str_array);

    // API sequence test completed successfully
    return 66;
}