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
//<ID> 485
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *num1 = cJSON_CreateNumber(1.0);
    cJSON *num2 = cJSON_CreateNumber(2.5);
    cJSON *label = cJSON_CreateString("alpha");
    cJSON_AddItemToArray(arr, num1);
    cJSON_AddItemToArray(arr, num2);
    cJSON_AddItemToObject(root, "label", label);
    cJSON_AddItemToObject(root, "values", arr);

    // step 2: Configure
    cJSON *meta = cJSON_CreateObject();
    cJSON *meta_name = cJSON_CreateString("example");
    cJSON_AddItemToObject(meta, "name", meta_name);
    cJSON_AddItemToObject(root, "meta", meta);

    // step 3: Operate
    cJSON *second_item = cJSON_GetArrayItem(arr, 1);
    double second_value = cJSON_GetNumberValue(second_item);
    cJSON *computed = cJSON_CreateNumber(second_value * 2.0 + 1.0);
    cJSON_AddItemToObject(root, "computed", computed);

    // step 4: Validate and Cleanup
    cJSON *computed_item = cJSON_GetObjectItem(root, "computed");
    double computed_value = cJSON_GetNumberValue(computed_item);
    (void)computed_value;
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}