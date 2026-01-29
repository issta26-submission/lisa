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
//<ID> 1223
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json_text = "{\"ints\":[10,20,30],\"meta\":{\"scale\":2}}";
    const char *parse_end = NULL;
    cJSON *parsed = cJSON_ParseWithOpts(json_text, &parse_end, 1);
    cJSON *root = cJSON_CreateObject();

    // step 2: Configure
    cJSON *ints = cJSON_GetObjectItem(parsed, "ints");
    int local_ints[3];
    local_ints[0] = (int)cJSON_GetNumberValue(cJSON_GetArrayItem(ints, 0));
    local_ints[1] = (int)cJSON_GetNumberValue(cJSON_GetArrayItem(ints, 1));
    local_ints[2] = (int)cJSON_GetNumberValue(cJSON_GetArrayItem(ints, 2));
    cJSON *copied_ints = cJSON_CreateIntArray(local_ints, 3);
    cJSON_AddItemToObject(root, "copied_ints", copied_ints);

    // step 3: Operate & Validate
    cJSON *meta = cJSON_GetObjectItem(parsed, "meta");
    cJSON *scale_item = cJSON_GetObjectItem(meta, "scale");
    double scale = cJSON_GetNumberValue(scale_item);
    double sum = (double)(local_ints[0] + local_ints[1] + local_ints[2]);
    cJSON *scaled = cJSON_CreateNumber(sum * scale);
    cJSON_AddItemToObject(root, "scaled_sum", scaled);

    // step 4: Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(root);
    cJSON_Delete(parsed);

    // API sequence test completed successfully
    return 66;
}