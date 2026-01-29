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
//<ID> 1118
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
    cJSON *n0 = cJSON_CreateNumber(10.0);
    cJSON *n1 = cJSON_CreateNumber(20.0);
    const char *label_text = "values_label";
    cJSON *label_ref = cJSON_CreateStringReference(label_text);

    // step 2: Configure
    cJSON_AddItemToArray(arr, n0);
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToObject(root, "nums", arr);
    cJSON_AddItemToObject(root, "label", label_ref);
    cJSON_AddTrueToObject(root, "active");

    // step 3: Operate & Validate
    cJSON *item1 = cJSON_GetArrayItem(arr, 1);
    double updated_value = cJSON_SetNumberHelper(item1, 42.5);
    cJSON_bool is_num = cJSON_IsNumber(item1);
    cJSON_AddNumberToObject(root, "item1_is_number", (double)is_num);
    cJSON_AddNumberToObject(root, "item1_value", updated_value);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}