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
//<ID> 950
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
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "items", items);
    cJSON_AddItemToObject(root, "meta", meta);

    // step 2: Configure
    cJSON *num1 = cJSON_CreateNumber(7.0);
    cJSON *num2 = cJSON_CreateNumber(3.14);
    cJSON *truth = cJSON_CreateTrue();
    cJSON_AddItemToArray(items, num1);
    cJSON_AddItemToArray(items, num2);
    cJSON_AddItemToArray(items, truth);
    cJSON *child = cJSON_CreateObject();
    cJSON_AddItemToObject(child, "value", cJSON_CreateNumber(99.0));
    cJSON_AddItemToObject(meta, "child", child);

    // step 3: Operate and Validate
    cJSON *child_ref = cJSON_CreateObjectReference(child);
    cJSON_AddItemToObject(root, "child_ref", child_ref);
    cJSON *second_item = cJSON_GetArrayItem(items, 1);
    double second_value = cJSON_GetNumberValue(second_item);
    cJSON *derived = cJSON_CreateNumber(second_value + 1.0);
    cJSON_AddItemToObject(root, "derived", derived);
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON_free(snapshot);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}