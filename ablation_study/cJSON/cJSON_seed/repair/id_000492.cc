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
//<ID> 492
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_Parse("{\"base\":{\"x\":1},\"list\":[10,20]}");
    cJSON *new_array = cJSON_CreateArray();
    cJSON *num100 = cJSON_CreateNumber(100.0);
    cJSON *num200 = cJSON_CreateNumber(200.0);
    cJSON_AddItemToArray(new_array, num100);
    cJSON_AddItemToArray(new_array, num200);

    // step 2: Configure
    cJSON *added_obj = cJSON_AddObjectToObject(root, "added_obj");
    cJSON *list = cJSON_GetObjectItem(root, "list");
    cJSON *item0 = cJSON_GetArrayItem(list, 0);
    cJSON *item1 = cJSON_GetArrayItem(list, 1);
    cJSON *dup0 = cJSON_Duplicate(item0, 1);
    cJSON *dup1 = cJSON_Duplicate(item1, 1);
    cJSON_AddItemToArray(new_array, dup0);
    cJSON_AddItemToArray(new_array, dup1);
    cJSON_AddItemToObject(added_obj, "injected", new_array);

    // step 3: Operate
    cJSON *label = cJSON_CreateString("merged");
    cJSON_AddItemToObject(root, "label", label);
    char *printed = cJSON_PrintUnformatted(root);
    cJSON_free(printed);

    // step 4: Validate and Cleanup
    cJSON *injected = cJSON_GetObjectItem(added_obj, "injected");
    cJSON *third = cJSON_GetArrayItem(injected, 2);
    double v = cJSON_GetNumberValue(third);
    (void)v;
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}