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
//<ID> 1476
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *child = NULL;
    cJSON *arr = NULL;
    cJSON *num_item = NULL;
    cJSON *str_item = NULL;
    cJSON *dup_child = NULL;
    cJSON *got_child = NULL;
    cJSON *got_array0 = NULL;
    cJSON *detached = NULL;
    double numeric_value = 0.0;
    char *string_value = NULL;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    child = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "child", child);
    str_item = cJSON_CreateString("hello");
    cJSON_AddItemToObject(child, "greet", str_item);
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "list", arr);
    num_item = cJSON_CreateNumber(42.0);
    cJSON_AddItemToArray(arr, num_item);

    // step 3: Operate / Validate
    dup_child = cJSON_Duplicate(child, 1);
    got_child = cJSON_GetObjectItem(root, "child");
    got_array0 = cJSON_GetArrayItem(arr, 0);
    numeric_value = cJSON_GetNumberValue(got_array0);
    string_value = cJSON_GetStringValue(cJSON_GetObjectItem(got_child, "greet"));
    detached = cJSON_DetachItemViaPointer(root, got_child);
    cJSON_AddItemToObject(root, "reinserted", detached);
    (void)numeric_value;
    (void)string_value;

    // step 4: Cleanup
    cJSON_Delete(dup_child);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}