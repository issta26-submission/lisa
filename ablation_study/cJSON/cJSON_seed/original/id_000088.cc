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
//<ID> 88
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
    cJSON_AddTrueToObject(root, "flag");
    cJSON *num1 = cJSON_CreateNumber(42.0);
    cJSON *num2 = cJSON_CreateNumber(3.14);
    cJSON_AddItemToArray(items, num1);
    cJSON_AddItemToArray(items, num2);

    // step 3: Operate and Validate
    char *printed = cJSON_Print(root);
    cJSON *detached = cJSON_DetachItemViaPointer(root, child);
    cJSON_bool items_is_array = cJSON_IsArray(items);
    int summary = (int)items_is_array + (int)printed[0] + (int)(detached != NULL);
    (void)summary;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(detached);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}