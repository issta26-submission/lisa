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
//<ID> 85
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
    cJSON *flag = cJSON_AddTrueToObject(root, "flag");

    // step 2: Configure
    cJSON *num = cJSON_CreateNumber(42.0);
    cJSON_AddItemToArray(items, num);
    cJSON *str = cJSON_CreateString("hello");
    cJSON_AddItemToArray(items, str);

    // step 3: Operate and Validate
    cJSON_bool items_is_array = cJSON_IsArray(items);
    cJSON *detached = cJSON_DetachItemViaPointer(root, child);
    cJSON_AddItemToArray(items, detached);
    char *printed_root = cJSON_Print(root);
    char *printed_detached = cJSON_Print(detached);
    int count = cJSON_GetArraySize(items);
    double first_num = cJSON_GetNumberValue(cJSON_GetArrayItem(items, 0));
    char *second_str = cJSON_GetStringValue(cJSON_GetArrayItem(items, 1));
    int summary = count + (int)items_is_array + (int)first_num + (second_str ? (int)second_str[0] : 0) + (flag ? 1 : 0);
    (void)summary;

    // step 4: Cleanup
    cJSON_free(printed_root);
    cJSON_free(printed_detached);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}