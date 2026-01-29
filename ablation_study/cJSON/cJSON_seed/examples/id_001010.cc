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
//<ID> 1010
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json[] = "{\"name\":\"unit\",\"values\":[1,2,3],\"meta\":{\"num\":42.5}}";
    size_t buffer_length = sizeof(json) - 1;
    cJSON *root = cJSON_ParseWithLength(json, buffer_length);

    // step 2: Configure
    cJSON *values = cJSON_GetObjectItem(root, "values");
    cJSON *detached = cJSON_DetachItemFromArray(values, 1);
    cJSON *meta = cJSON_GetObjectItem(root, "meta");

    // step 3: Operate & Validate
    cJSON *name_item = cJSON_GetObjectItem(root, "name");
    char *name_str = cJSON_GetStringValue(name_item);
    cJSON *name_copy = cJSON_CreateString(name_str);
    cJSON_AddItemToObject(root, "name_copy", name_copy);
    cJSON *num_item = cJSON_GetObjectItem(meta, "num");
    double num_val = cJSON_GetNumberValue(num_item);
    cJSON *num_plus_one = cJSON_CreateNumber(num_val + 1.0);
    cJSON_AddItemToObject(root, "num_plus_one", num_plus_one);
    cJSON_AddItemToObject(root, "removed", detached);

    // step 4: Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}