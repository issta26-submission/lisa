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
//<ID> 613
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
    cJSON *n1 = cJSON_CreateNumber(12.5);
    cJSON_AddItemToObject(root, "n1", n1);
    cJSON *n2 = cJSON_CreateNumber(7.25);
    cJSON_AddItemToObject(root, "n2", n2);
    cJSON *temp = cJSON_CreateString("to_be_deleted");
    cJSON_AddItemToObject(root, "temp", temp);

    // step 2: Configure
    cJSON *n1_ref = cJSON_GetObjectItem(root, "n1");
    cJSON_AddItemReferenceToArray(items, n1_ref);
    cJSON *detached_n2 = cJSON_DetachItemFromObjectCaseSensitive(root, "n2");
    cJSON_AddItemToArray(items, detached_n2);

    // step 3: Operate
    cJSON *item0 = cJSON_GetArrayItem(items, 0);
    cJSON *item1 = cJSON_GetArrayItem(items, 1);
    double v0 = cJSON_GetNumberValue(item0);
    double v1 = cJSON_GetNumberValue(item1);
    double total = v0 + v1;
    cJSON *sum_item = cJSON_CreateNumber(total);
    cJSON_AddItemToObject(root, "sum", sum_item);

    // step 4: Validate & Cleanup
    char *printed = cJSON_PrintUnformatted(root);
    cJSON_DeleteItemFromObject(root, "temp");
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}