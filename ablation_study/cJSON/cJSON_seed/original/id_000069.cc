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
//<ID> 69
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *items = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", items);
    cJSON *child = cJSON_CreateObject();
    cJSON_AddStringToObject(child, "name", "original");
    cJSON_AddItemToObject(root, "child", child);

    // step 2: Configure
    cJSON *num = cJSON_CreateNumber(123.0);
    cJSON_AddItemToArray(items, num);
    cJSON *child_ref = cJSON_CreateObjectReference(child);
    cJSON_AddItemReferenceToArray(items, child_ref);

    // step 3: Operate and Validate
    cJSON *first = cJSON_GetArrayItem(items, 0);
    cJSON *second = cJSON_GetArrayItem(items, 1);
    cJSON_bool same = cJSON_Compare(child, second, 1);
    cJSON_bool second_is_obj = cJSON_IsObject(second);
    int count = cJSON_GetArraySize(items);
    double first_val = cJSON_GetNumberValue(first);
    int summary = (int)count + (int)first_val + (int)same + (int)second_is_obj;
    (void)summary;

    // step 4: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}