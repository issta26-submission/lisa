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
//<ID> 67
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
    cJSON *obj = cJSON_CreateObject();
    cJSON_AddNumberToObject(obj, "value", 7.5);
    cJSON_AddStringToObject(obj, "name", "alpha");

    // step 2: Configure
    cJSON_AddItemToArray(items, obj);
    cJSON *obj_ref = cJSON_CreateObjectReference(obj);
    cJSON_AddItemReferenceToArray(items, obj_ref);
    cJSON *dup_obj = cJSON_Duplicate(obj, 1);
    cJSON_AddItemToObject(root, "duplicate", dup_obj);

    // step 3: Operate and Validate
    cJSON *first = cJSON_GetArrayItem(items, 0);
    cJSON *second = cJSON_GetArrayItem(items, 1);
    cJSON_bool first_is_obj = cJSON_IsObject(first);
    cJSON_bool second_is_obj = cJSON_IsObject(second);
    cJSON_bool objects_equal = cJSON_Compare(first, second, 1);
    cJSON *value_node = cJSON_GetObjectItem(first, "value");
    double value_num = cJSON_GetNumberValue(value_node);
    int summary = (int)first_is_obj + (int)second_is_obj + (int)objects_equal + (int)value_num;
    (void)summary;

    // step 4: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}