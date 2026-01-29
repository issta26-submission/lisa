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
//<ID> 42
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *version = cJSON_Version();
    cJSON *root = cJSON_CreateObject();
    cJSON_AddStringToObject(root, "version", version);
    cJSON *values = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "values", values);

    // step 2: Configure
    cJSON *pi_item = cJSON_AddNumberToObject(root, "pi", 3.14159);
    cJSON *true_item = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "enabled", true_item);
    cJSON *false_item = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "disabled", false_item);
    cJSON *n1 = cJSON_CreateNumber(1.0);
    cJSON_bool inserted0 = cJSON_InsertItemInArray(values, 0, n1);
    cJSON *n2 = cJSON_CreateNumber(2.0);
    cJSON_bool inserted1 = cJSON_InsertItemInArray(values, 1, n2);

    // step 3: Operate and Validate
    cJSON *got_pi = cJSON_GetObjectItem(root, "pi");
    double pi_val = cJSON_GetNumberValue(got_pi);
    cJSON *got_enabled = cJSON_GetObjectItem(root, "enabled");
    cJSON_bool enabled_flag = cJSON_IsTrue(got_enabled);
    int values_count = cJSON_GetArraySize(values);
    cJSON *first_val_item = cJSON_GetArrayItem(values, 0);
    double first_val = cJSON_GetNumberValue(first_val_item);
    cJSON *dup_root = cJSON_Duplicate(root, 1);
    cJSON_bool equal = cJSON_Compare(root, dup_root, 1);
    (void)pi_val;
    (void)enabled_flag;
    (void)values_count;
    (void)first_val;
    (void)inserted0;
    (void)inserted1;
    (void)equal;

    // step 4: Cleanup
    cJSON_Delete(dup_root);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}