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
//<ID> 1134
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    double numbers[3] = { 1.0, 2.5, -3.14 };
    cJSON *array = cJSON_CreateDoubleArray(numbers, 3);
    cJSON *flag = cJSON_CreateTrue();
    cJSON *meta = cJSON_CreateObject();
    cJSON *label = cJSON_CreateString("example_payload");

    // step 2: Configure
    cJSON_AddItemToObject(root, "values", array);
    cJSON_AddItemToObjectCS(root, "enabled", flag);
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddItemToObject(meta, "label", label);

    // step 3: Operate & Validate
    cJSON_bool enabled = cJSON_IsTrue(flag);
    cJSON_AddNumberToObject(root, "enabled_flag_numeric", (double)enabled);
    cJSON *array_copy = cJSON_Duplicate(array, 1);
    cJSON_AddItemToObject(meta, "values_copy", array_copy);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}