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
//<ID> 955
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
    cJSON *num1 = cJSON_CreateNumber(3.1415);
    cJSON *truth = cJSON_CreateTrue();
    cJSON *num2 = cJSON_CreateNumber(42.0);
    cJSON_AddItemToArray(items, num1);
    cJSON_AddItemToArray(items, truth);
    cJSON_AddItemToArray(items, num2);
    cJSON *num1_ref = cJSON_CreateObjectReference(num1);
    cJSON_AddItemToObject(root, "first_ref", num1_ref);

    // step 2: Configure
    cJSON_AddNumberToObject(root, "direct_number", 1.618);
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddStringToObject(meta, "note", "configuration complete");

    // step 3: Operate and Validate
    cJSON *second_item = cJSON_GetArrayItem(items, 1);
    cJSON *first_item = cJSON_GetArrayItem(items, 0);
    double first_val = cJSON_GetNumberValue(first_item);
    cJSON_bool first_is_number = cJSON_IsNumber(first_item);
    char *snapshot = cJSON_PrintUnformatted(root);
    (void)second_item;
    (void)first_val;
    (void)first_is_number;
    (void)snapshot;

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}