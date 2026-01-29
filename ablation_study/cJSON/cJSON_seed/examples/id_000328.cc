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
//<ID> 328
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *metrics = cJSON_CreateObject();
    cJSON *values = cJSON_CreateArray();
    cJSON *num_item = cJSON_CreateNumber(1.5);
    cJSON_AddItemToArray(values, num_item);
    cJSON_AddItemToObject(root, "metrics", metrics);
    cJSON_AddItemToObject(metrics, "values", values);
    cJSON_AddNumberToObject(metrics, "initial_count", 1.0);

    // step 2: Configure
    double new_val = cJSON_SetNumberHelper(num_item, 2.75);
    cJSON_AddNumberToObject(root, "set_result", new_val);
    cJSON *null_item = cJSON_CreateNull();
    cJSON_AddItemToObject(root, "maybe_null", null_item);
    cJSON_AddItemToObject(root, "extra_info", cJSON_CreateString("configuration_ok"));

    // step 3: Operate & Validate
    char *snapshot = cJSON_Print(root);
    cJSON_AddStringToObject(root, "snapshot_before_delete", snapshot);
    cJSON_DeleteItemFromObjectCaseSensitive(root, "maybe_null");
    double observed = cJSON_GetNumberValue(cJSON_GetArrayItem(values, 0));
    cJSON_AddNumberToObject(root, "observed_value", observed);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}