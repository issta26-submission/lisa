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
//<ID> 277
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json = "{\"value\": 12.5, \"name\": \"test\"}";
    cJSON *root = cJSON_Parse(json);

    // step 2: Configure
    cJSON *val_item = cJSON_GetObjectItem(root, "value");
    double val = cJSON_GetNumberValue(val_item);
    cJSON *double_item = cJSON_AddNumberToObject(root, "value_double", val * 2.0);
    cJSON *true_item = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "enabled", true_item);

    // step 3: Operate and Validate
    cJSON *copy_item = cJSON_AddNumberToObject(root, "value_copy", val);
    char *out = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}