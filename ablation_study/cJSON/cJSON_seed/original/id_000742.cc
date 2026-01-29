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
//<ID> 742
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json_text = "{\"name\":\"test\",\"values\":[1,2,3],\"meta\":{\"a\":1}}";
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithOpts(json_text, &parse_end, 1);

    // step 2: Configure
    cJSON *values = cJSON_GetObjectItem(root, "values");
    int orig_size = cJSON_GetArraySize(values);
    cJSON_AddNumberToObject(root, "orig_values_count", (double)orig_size);
    cJSON *meta = cJSON_GetObjectItem(root, "meta");
    cJSON *meta_dup = cJSON_Duplicate(meta, 1);
    cJSON_AddItemToObject(root, "meta_copy", meta_dup);

    // step 3: Operate
    cJSON *flag = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "enabled", flag);
    cJSON_AddNullToObject(root, "missing");
    int final_size = cJSON_GetArraySize(values);
    cJSON_AddNumberToObject(root, "final_values_count", (double)final_size);
    char *printed = cJSON_PrintUnformatted(root);

    // step 4: Validate & Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}