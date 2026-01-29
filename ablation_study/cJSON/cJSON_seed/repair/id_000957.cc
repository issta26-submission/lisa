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
//<ID> 957
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
    cJSON *num = cJSON_CreateNumber(123.0);
    cJSON_AddItemToArray(items, num);
    cJSON *t = cJSON_CreateTrue();
    cJSON_AddItemToArray(items, t);

    // step 2: Configure
    cJSON *child = cJSON_CreateObject();
    cJSON_AddStringToObject(child, "child_name", "ref_target");
    cJSON_AddNumberToObject(child, "child_val", 7.0);
    cJSON *ref = cJSON_CreateObjectReference(child);
    cJSON_AddItemToObject(root, "child_ref", ref);
    cJSON_AddItemToObject(root, "child", child);

    // step 3: Operate and Validate
    cJSON *second_item = cJSON_GetArrayItem(items, 1);
    cJSON_bool second_is_true = cJSON_IsTrue(second_item);
    cJSON *first_item = cJSON_GetArrayItem(items, 0);
    double first_value = cJSON_GetNumberValue(first_item);
    char *snapshot = cJSON_PrintUnformatted(root);
    (void)second_is_true;
    (void)first_value;

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}