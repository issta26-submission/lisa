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
//<ID> 1048
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
    cJSON *active = cJSON_CreateBool(1);
    cJSON *label = cJSON_CreateString("v1");
    cJSON *values = cJSON_CreateArray();
    cJSON *n1 = cJSON_CreateNumber(10.0);
    cJSON *n2 = cJSON_CreateNumber(20.0);
    cJSON *n3 = cJSON_CreateNumber(30.0);
    cJSON_AddItemToArray(values, n1);
    cJSON_AddItemToArray(values, n2);
    cJSON_AddItemToArray(values, n3);

    // step 2: Configure
    cJSON_AddItemToObjectCS(config, "active", active);
    cJSON_AddItemToObjectCS(config, "label", label);
    cJSON_AddItemToObjectCS(config, "values", values);
    cJSON_AddItemToObjectCS(root, "config", config);
    cJSON_SetValuestring(label, "v1_modified");

    // step 3: Operate
    cJSON_bool is_array = cJSON_IsArray(values);
    cJSON *array_flag = cJSON_CreateBool(is_array);
    cJSON_AddItemToObjectCS(root, "values_is_array", array_flag);
    char *pretty = cJSON_Print(root);
    char *compact = cJSON_PrintUnformatted(root);
    char *prebuf = (char *)cJSON_malloc(256);
    memset(prebuf, 0, 256);
    cJSON_PrintPreallocated(root, prebuf, 256, 1);

    // step 4: Validate & Cleanup
    cJSON_free(pretty);
    cJSON_free(compact);
    cJSON_free(prebuf);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}