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
//<ID> 952
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

    // step 2: Configure
    cJSON *n1 = cJSON_CreateNumber(1.5);
    cJSON *n2 = cJSON_CreateNumber(2.5);
    cJSON_AddItemToArray(items, n1);
    cJSON_AddItemToArray(items, n2);
    cJSON *truth = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "enabled", truth);
    cJSON *child = cJSON_CreateObject();
    cJSON_AddStringToObject(child, "label", "child_node");
    cJSON_AddNumberToObject(child, "value", 7.0);
    cJSON *ref = cJSON_CreateObjectReference(child);
    cJSON_AddItemToObject(root, "child_ref", ref);

    // step 3: Operate and Validate
    cJSON *second = cJSON_GetArrayItem(items, 1);
    double second_val = cJSON_GetNumberValue(second);
    cJSON *first = cJSON_GetArrayItem(items, 0);
    double first_val = cJSON_GetNumberValue(first);
    char *snapshot = cJSON_PrintUnformatted(root);
    (void)first_val;
    (void)second_val;

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(root);
    cJSON_Delete(child);
    // API sequence test completed successfully
    return 66;
}