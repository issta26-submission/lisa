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
//<ID> 1243
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
    cJSON *num1 = cJSON_CreateNumber(3.14);
    cJSON *num2 = cJSON_CreateNumber(2.0);
    cJSON *label = cJSON_CreateString("alpha");

    // step 2: Configure
    cJSON_AddItemToArray(values, num1);
    cJSON_AddItemToArray(values, num2);
    cJSON_AddItemToArray(values, label);
    cJSON_AddItemToObject(root, "values", values);
    cJSON_AddStringToObject(root, "name", "example");
    cJSON_bool array_flag = cJSON_IsArray(values);
    cJSON_AddNumberToObject(root, "is_array", (double)array_flag);
    cJSON *values_copy = cJSON_Duplicate(values, 1);
    cJSON_AddItemToObject(root, "values_copy", values_copy);

    // step 3: Operate & Validate
    char *unformatted = cJSON_PrintUnformatted(root);
    cJSON_free(unformatted);

    // step 4: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}