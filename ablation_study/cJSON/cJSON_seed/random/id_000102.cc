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
//<ID> 102
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *s1 = cJSON_CreateString("one");
    cJSON *s2 = cJSON_CreateString("two");
    cJSON *child = cJSON_CreateString("old");
    cJSON *detached = (cJSON *)0;
    cJSON *retrieved = (cJSON *)0;
    cJSON *replacement = (cJSON *)0;
    int size = 0;
    double final_value = 0.0;

    // step 2: Setup
    cJSON_AddItemToArray(arr, s1);
    cJSON_AddItemToArray(arr, s2);
    cJSON_AddItemToObject(root, "items", arr);
    cJSON_AddItemToObject(root, "child", child);

    // step 3: Operate
    retrieved = cJSON_GetObjectItem(root, "items");
    size = cJSON_GetArraySize(retrieved);
    detached = cJSON_DetachItemFromObject(root, "child");
    cJSON_AddItemToObject(root, "child", detached);
    replacement = cJSON_CreateNumber((double)size);
    cJSON_ReplaceItemInObject(root, "child", replacement);

    // step 4: Validate
    final_value = cJSON_GetNumberValue(cJSON_GetObjectItem(root, "child"));
    cJSON_AddNumberToObject(root, "child_value_recorded", final_value);

    // step 5: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}