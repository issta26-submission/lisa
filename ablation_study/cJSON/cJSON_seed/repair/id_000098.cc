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
//<ID> 98
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *config = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "config", config);
    cJSON *name = cJSON_CreateString("initial");
    cJSON_AddItemToObject(config, "name", name);
    cJSON *values = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "values", values);
    cJSON *n1 = cJSON_CreateNumber(3.0);
    cJSON_AddItemToArray(values, n1);
    cJSON *n2 = cJSON_CreateNumber(4.5);
    cJSON_AddItemToArray(values, n2);

    // step 2: Configure
    char *set_ret = cJSON_SetValuestring(name, "updated");
    cJSON *dup_config = cJSON_Duplicate(config, 1);
    cJSON_AddItemToObject(root, "backup", dup_config);
    cJSON *got_values = cJSON_GetObjectItem(root, "values");

    // step 3: Operate and Validate
    int sz = cJSON_GetArraySize(got_values);
    cJSON *first = cJSON_GetArrayItem(got_values, 0);
    double first_val = cJSON_GetNumberValue(first);
    cJSON_AddNumberToObject(root, "values_size", (double)sz);
    cJSON_AddNumberToObject(root, "first_value", first_val);
    if (set_ret) { /* intentionally no branches altering flow; use set_ret for data flow */ }
    cJSON_AddNumberToObject(root, "first_char_code", (double)(unsigned char)set_ret[0]);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}