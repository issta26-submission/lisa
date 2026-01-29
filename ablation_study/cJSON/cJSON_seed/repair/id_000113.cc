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
//<ID> 113
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
    cJSON *values = cJSON_CreateArray();
    cJSON_AddItemToObject(config, "values", values);
    cJSON *n1 = cJSON_CreateNumber(3.5);
    cJSON_AddItemToArray(values, n1);
    cJSON *n2 = cJSON_CreateNumber(4.5);
    cJSON_AddItemToArray(values, n2);

    // step 2: Configure
    cJSON *dup_config = cJSON_Duplicate(config, 1);
    cJSON_AddItemToObject(root, "backup", dup_config);
    cJSON_AddNumberToObject(root, "initial_count", (double)cJSON_GetArraySize(values));

    // step 3: Operate and Validate
    cJSON *got_config = cJSON_GetObjectItem(root, "config");
    cJSON *got_values = cJSON_GetObjectItem(got_config, "values");
    cJSON *first = cJSON_GetArrayItem(got_values, 0);
    double first_val = cJSON_GetNumberValue(first);
    cJSON *dup_values = cJSON_GetObjectItem(dup_config, "values");
    cJSON *dup_first = cJSON_GetArrayItem(dup_values, 0);
    double dup_first_val = cJSON_GetNumberValue(dup_first);
    double sum = first_val + dup_first_val;
    cJSON_AddNumberToObject(root, "sum_first_values", sum);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}