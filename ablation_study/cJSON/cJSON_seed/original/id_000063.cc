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
//<ID> 63
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *child = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "child", child);
    cJSON_AddItemToObject(root, "list", arr);
    cJSON *num = cJSON_CreateNumber(123.0);
    cJSON_AddItemToObject(child, "value", num);
    cJSON *str = cJSON_CreateString("hello");
    cJSON_AddItemToObject(child, "greet", str);

    // step 2: Configure
    cJSON *child_ref = cJSON_CreateObjectReference(child);
    cJSON_AddItemReferenceToArray(arr, child_ref);
    cJSON_AddItemToArray(arr, cJSON_CreateNumber(7.0));
    cJSON_AddItemToArray(arr, cJSON_CreateString("extra"));

    // step 3: Operate and Validate
    cJSON *first = cJSON_GetArrayItem(arr, 0);
    cJSON_bool first_is_object = cJSON_IsObject(first);
    cJSON_bool equal = cJSON_Compare(first, child, 1);
    cJSON *value_item = cJSON_GetObjectItem(first, "value");
    double value_num = cJSON_GetNumberValue(value_item);
    int summary = (int)first_is_object + (int)equal + (int)value_num;
    (void)summary;

    // step 4: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}