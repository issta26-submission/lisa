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
//<ID> 970
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_AddObjectToObject(root, "meta");
    cJSON *items = cJSON_CreateArray();
    cJSON_AddItemToObjectCS(root, "items", items);

    // step 2: Configure
    cJSON *num1 = cJSON_CreateNumber(42.0);
    cJSON_AddItemToArray(items, num1);
    cJSON *str1 = cJSON_CreateString("example");
    cJSON_AddItemToArray(items, str1);
    cJSON *child = cJSON_CreateObject();
    cJSON_AddStringToObject(child, "name", "detachable");
    cJSON_AddNumberToObject(child, "id", 7.0);
    cJSON_AddItemToObjectCS(root, "child", child);

    // step 3: Operate and Validate
    cJSON *detached = cJSON_DetachItemFromObject(root, "child");
    cJSON_AddItemToObjectCS(meta, "orphan", detached);
    char buffer[512];
    memset(buffer, 0, sizeof(buffer));
    cJSON_bool printed = cJSON_PrintPreallocated(root, buffer, (int)sizeof(buffer), 1);
    (void)printed;
    cJSON *first_item = cJSON_GetArrayItem(items, 0);
    double first_value = cJSON_GetNumberValue(first_item);
    (void)first_value;

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}