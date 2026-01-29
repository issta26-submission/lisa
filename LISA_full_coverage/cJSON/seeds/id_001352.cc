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
//<ID> 1352
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    float nums[3] = {1.5f, 2.5f, -3.25f};
    cJSON *root = cJSON_CreateObject();
    cJSON *cfg = cJSON_AddObjectToObject(root, "config");
    cJSON *vals = cJSON_CreateFloatArray(nums, 3);
    cJSON_AddItemToObject(cfg, "values", vals);
    cJSON_AddTrueToObject(cfg, "enabled");
    cJSON *opt = cJSON_CreateNull();
    cJSON_AddItemToObject(cfg, "optional", opt);

    // step 2: Configure
    cJSON *meta = cJSON_AddObjectToObject(cfg, "meta");
    cJSON_AddStringToObject(meta, "note", "generated");
    cJSON_AddNumberToObject(meta, "version", 1.0);

    // step 3: Operate
    cJSON *values_array = cJSON_GetObjectItem(cfg, "values");
    cJSON *second = cJSON_GetArrayItem(values_array, 1);
    double extracted = cJSON_GetNumberValue(second);
    cJSON *marker = cJSON_CreateNumber(extracted);
    cJSON_AddItemToObject(root, "extracted", marker);

    // step 4: Validate & Cleanup
    char *printed = cJSON_PrintUnformatted(root);
    size_t printed_len = strlen(printed);
    char *buf = (char *)cJSON_malloc(printed_len + 1);
    memset(buf, 0, printed_len + 1);
    memcpy(buf, printed, printed_len + 1);
    cJSON_Minify(buf);
    cJSON_free(printed);
    cJSON_free(buf);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}