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
//<ID> 875
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
    cJSON_AddItemToObject(root, "values", values);
    cJSON *n1 = cJSON_CreateNumber(10.5);
    cJSON *n2 = cJSON_CreateNumber(20.25);
    cJSON_AddItemToArray(values, n1);
    cJSON_AddItemToArray(values, n2);

    // step 2: Configure
    cJSON_AddNumberToObject(config, "threshold", 5.0);
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddStringToObject(meta, "name", "sensorA");
    cJSON_AddItemToObject(config, "meta", meta);

    // step 3: Operate and Validate
    cJSON *first_val = cJSON_GetArrayItem(values, 0);
    double orig_val = cJSON_GetNumberValue(first_val);
    double set_result = cJSON_SetNumberHelper(first_val, orig_val * 2.0);
    double new_val = cJSON_GetNumberValue(first_val);
    cJSON *threshold_item = cJSON_GetObjectItem(config, "threshold");
    double threshold_val = cJSON_GetNumberValue(threshold_item);
    double combined = new_val + threshold_val + set_result - set_result; // use values meaningfully
    cJSON_AddNumberToObject(root, "computed", combined);
    char *snapshot = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}