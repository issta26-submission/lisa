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
//<ID> 640
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *arr = (cJSON *)0;
    cJSON *extra_str = (cJSON *)0;
    cJSON *true_item = (cJSON *)0;
    cJSON *retrieved = (cJSON *)0;
    cJSON_bool is_true = 0;
    const char *const initial[] = { "one", "two", "three" };
    int ptr_non_null = 0;

    // step 2: Initialize
    arr = cJSON_CreateStringArray(initial, 3);
    true_item = cJSON_CreateTrue();
    extra_str = cJSON_CreateString("four");

    // step 3: Configure
    cJSON_AddItemToArray(arr, extra_str);

    // step 4: Operate
    retrieved = cJSON_GetArrayItem(arr, 1);
    is_true = cJSON_IsTrue(true_item);

    // step 5: Validate
    ptr_non_null = (retrieved != (cJSON *)0);
    (void)is_true;
    (void)ptr_non_null;
    (void)retrieved;
    (void)extra_str;
    (void)initial;

    // step 6: Cleanup
    cJSON_Delete(arr);
    cJSON_Delete(true_item);

    // API sequence test completed successfully
    return 66;
}