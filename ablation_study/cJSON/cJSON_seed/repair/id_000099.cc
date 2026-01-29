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
//<ID> 99
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
    cJSON_AddItemToObject(config, "values", values);
    cJSON *v1 = cJSON_CreateNumber(10.0);
    cJSON_AddItemToArray(values, v1);
    cJSON *v2 = cJSON_CreateNumber(20.0);
    cJSON_AddItemToArray(values, v2);

    // step 2: Configure
    char *retptr = cJSON_SetValuestring(name, "updated");
    cJSON *dup_config = cJSON_Duplicate(config, 1);
    cJSON_AddItemToObject(root, "backup", dup_config);

    // step 3: Operate and Validate
    cJSON *got_config = cJSON_GetObjectItem(root, "config");
    cJSON *got_values = cJSON_GetObjectItem(got_config, "values");
    int size_orig = cJSON_GetArraySize(got_values);
    cJSON *dup_values = cJSON_GetObjectItem(dup_config, "values");
    int size_dup = cJSON_GetArraySize(dup_values);
    double combined = (double)(size_orig + size_dup);
    cJSON_AddNumberToObject(root, "combined_size", combined);
    cJSON_AddNumberToObject(root, "first_char_code", (double)(unsigned char)retptr[0]);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}