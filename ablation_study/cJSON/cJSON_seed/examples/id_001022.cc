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
//<ID> 1022
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *values = cJSON_CreateArray();
    cJSON *n0 = cJSON_CreateNumber(10.0);
    cJSON *n2 = cJSON_CreateNumber(30.0);
    cJSON_AddItemToArray(values, n0);
    cJSON_AddItemToArray(values, n2);
    cJSON_AddItemToObject(root, "values", values);
    cJSON_AddNumberToObject(root, "initial_count", 2.0);

    // step 2: Configure
    cJSON *n1 = cJSON_CreateNumber(20.0);
    cJSON_InsertItemInArray(values, 1, n1);
    cJSON_AddNumberToObject(root, "insert_index", 1.0);

    // step 3: Operate & Validate
    cJSON *item_at_1 = cJSON_GetArrayItem(values, 1);
    double value_at_1 = cJSON_GetNumberValue(item_at_1);
    cJSON_AddNumberToObject(root, "value_at_1", value_at_1);
    cJSON_bool root_is_object = cJSON_IsObject(root);
    cJSON_AddNumberToObject(root, "is_object_flag", (double)root_is_object);
    char *out = cJSON_Print(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}