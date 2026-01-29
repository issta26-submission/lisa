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
//<ID> 1072
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json[] = "{\"values\":[1,2,3],\"meta\":{\"count\":3}}";
    size_t json_len = sizeof(json) - 1;
    cJSON *root = cJSON_ParseWithLength(json, json_len);

    // step 2: Configure
    double replacements[] = {1.1, 2.2, 3.3};
    cJSON *double_arr = cJSON_CreateDoubleArray(replacements, 3);
    cJSON_ReplaceItemInObjectCaseSensitive(root, "values", double_arr);
    cJSON *meta = cJSON_GetObjectItem(root, "meta");
    cJSON *new_count = cJSON_CreateNumber(3.0);
    cJSON_ReplaceItemInObjectCaseSensitive(meta, "count", new_count);

    // step 3: Operate
    cJSON_AddTrueToObject(root, "active");
    cJSON *first_val = cJSON_GetArrayItem(cJSON_GetObjectItem(root, "values"), 0);
    double extracted = cJSON_GetNumberValue(first_val);
    char *unformatted = cJSON_PrintUnformatted(root);
    char *prealloc = (char *)cJSON_malloc(256);
    memset(prealloc, 0, 256);
    cJSON_PrintPreallocated(root, prealloc, 256, 1);

    // step 4: Validate & Cleanup
    (void)extracted;
    cJSON_free(unformatted);
    cJSON_free(prealloc);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}