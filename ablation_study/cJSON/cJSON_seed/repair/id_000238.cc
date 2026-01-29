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
//<ID> 238
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *num = cJSON_CreateNumber(21.0);
    cJSON *str = cJSON_CreateString("hello");
    cJSON *false_item = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "items", arr);
    cJSON_AddItemToObject(root, "enabled", false_item);

    // step 2: Configure
    cJSON_bool inserted1 = cJSON_InsertItemInArray(arr, 0, num);
    cJSON_bool inserted2 = cJSON_InsertItemInArray(arr, 1, str);
    cJSON *subobj = cJSON_CreateObject();
    cJSON_AddItemToObject(subobj, "label", cJSON_CreateString("sub"));
    cJSON_bool inserted3 = cJSON_InsertItemInArray(arr, 2, subobj);

    // step 3: Operate
    cJSON *retrieved_arr = cJSON_GetObjectItem(root, "items");
    int count = cJSON_GetArraySize(retrieved_arr);
    cJSON *first = cJSON_GetArrayItem(retrieved_arr, 0);
    double val = cJSON_GetNumberValue(first);
    cJSON *doubled = cJSON_CreateNumber(val * 2.0);
    cJSON_AddItemToObject(root, "double_answer", doubled);
    cJSON *flag = cJSON_GetObjectItem(root, "enabled");
    cJSON_bool is_false = cJSON_IsFalse(flag);
    cJSON *flag_copy = cJSON_CreateBool(is_false);
    cJSON_AddItemToObject(root, "is_false_copy", flag_copy);

    // step 4: Validate and Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}