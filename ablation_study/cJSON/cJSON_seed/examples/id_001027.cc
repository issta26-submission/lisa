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
//<ID> 1027
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
    cJSON_AddItemToObject(root, "values", values);
    cJSON_AddNumberToObject(root, "initial_count", 2.0);
    cJSON_AddItemToArray(values, cJSON_CreateNumber(1.0));
    cJSON_AddItemToArray(values, cJSON_CreateNumber(3.0));

    // step 2: Configure
    cJSON *inserted = cJSON_CreateNumber(2.0);
    cJSON_InsertItemInArray(values, 1, inserted);
    cJSON_bool root_is_object = cJSON_IsObject(root);

    // step 3: Operate & Validate
    cJSON *middle = cJSON_GetArrayItem(values, 1);
    double middle_val = cJSON_GetNumberValue(middle);
    cJSON_AddNumberToObject(root, "extracted_mid", middle_val);
    char *printed = cJSON_Print(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}