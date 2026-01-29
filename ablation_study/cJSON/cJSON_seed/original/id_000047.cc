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
//<ID> 47
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
    cJSON *arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "array", arr);
    cJSON_AddNumberToObject(root, "initial", 1.0);
    cJSON *true_item = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "enabled", true_item);
    cJSON *false_item = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "disabled", false_item);
    cJSON_AddNumberToObject(root, "version_flag", 0.0);

    // step 2: Configure
    cJSON_AddNumberToObject(root, "pi", 3.14159265);
    cJSON *n1 = cJSON_CreateNumber(10.0);
    cJSON *n2 = cJSON_CreateNumber(20.0);
    cJSON_AddItemToArray(arr, n1);
    cJSON_InsertItemInArray(arr, 0, n2);

    // step 3: Operate and Validate
    int size = cJSON_GetArraySize(arr);
    cJSON *first = cJSON_GetArrayItem(arr, 0);
    double first_val = cJSON_GetNumberValue(first);
    cJSON *pi_item = cJSON_GetObjectItem(root, "pi");
    double pi_val = cJSON_GetNumberValue(pi_item);
    cJSON *enabled_item = cJSON_GetObjectItem(root, "enabled");
    cJSON_bool enabled_is_true = cJSON_IsTrue(enabled_item);
    cJSON_bool has_initial = cJSON_HasObjectItem(root, "initial");

    // step 4: Cleanup
    cJSON_Delete(root);
    (void)version;
    (void)size;
    (void)first_val;
    (void)pi_val;
    (void)enabled_is_true;
    (void)has_initial;

    // API sequence test completed successfully
    return 66;
}