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
//<ID> 498
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json[] = "{\"data\":[1,2,3],\"meta\":{}}";
    cJSON *root = cJSON_Parse(json);
    cJSON *data = cJSON_GetObjectItem(root, "data");
    cJSON *meta = cJSON_GetObjectItem(root, "meta");
    cJSON_bool data_is_array = cJSON_IsArray(data);
    cJSON *first_item = cJSON_GetArrayItem(data, 0);
    double first_val = cJSON_GetNumberValue(first_item);

    // step 2: Configure
    cJSON_AddNullToObject(meta, "terminated");
    cJSON_AddFalseToObject(meta, "valid");
    cJSON *summary = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "summary", summary);
    double count_val = (double)cJSON_GetArraySize(data) + (double)data_is_array;
    cJSON *count_num = cJSON_CreateNumber(count_val);
    cJSON_AddItemToObject(summary, "count", count_num);

    // step 3: Operate
    char *snapshot = cJSON_PrintUnformatted(root);
    double reported_count = cJSON_GetNumberValue(cJSON_GetObjectItem(summary, "count"));
    double combined = first_val + reported_count;
    (void)combined;

    // step 4: Validate & Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}