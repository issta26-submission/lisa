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
//<ID> 421
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *raw_json = "{ \"items\": [7, 3.5], \"base\": 10 }";
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithOpts(raw_json, &parse_end, 1);

    // step 2: Configure
    cJSON *items = cJSON_GetObjectItem(root, "items");
    cJSON *base_item = cJSON_GetObjectItem(root, "base");
    cJSON *first_item = cJSON_GetArrayItem(items, 0);
    cJSON_bool items_are_array = cJSON_IsArray(items);
    double base_val = cJSON_GetNumberValue(base_item);
    double first_val = cJSON_GetNumberValue(first_item);
    double combined_val = base_val + first_val;
    cJSON_AddNumberToObject(root, "combined", combined_val);
    cJSON_AddBoolToObject(root, "items_are_array", items_are_array);

    // step 3: Operate & Validate
    double retrieved_combined = cJSON_GetNumberValue(cJSON_GetObjectItem(root, "combined"));
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON_AddStringToObject(root, "snapshot", snapshot);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}