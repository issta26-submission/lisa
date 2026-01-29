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
//<ID> 903
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *null_item = cJSON_CreateNull();
    cJSON_AddItemToObject(root, "nullable", null_item);
    cJSON_AddBoolToObject(root, "enabled", 1);
    cJSON *num = cJSON_CreateNumber(42.0);
    cJSON_AddItemToObject(root, "value", num);
    cJSON *name = cJSON_CreateString("sensor-x");
    cJSON_AddItemToObject(root, "name", name);

    // step 2: Configure
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddItemToObject(meta, "threshold", cJSON_CreateNumber(3.14));
    cJSON_AddItemToObject(meta, "status", cJSON_CreateString("ok"));

    // step 3: Operate & Validate
    cJSON *got_val = cJSON_GetObjectItem(root, "value");
    double original = cJSON_GetNumberValue(got_val);
    double scaled = cJSON_SetNumberHelper(got_val, original * 2.5);
    cJSON *dup_val = cJSON_Duplicate(got_val, 1);
    cJSON_AddItemToObject(root, "value_scaled", dup_val);
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON_Minify(snapshot);
    cJSON *snapshot_str = cJSON_CreateString(snapshot);
    cJSON_AddItemToObject(root, "snapshot", snapshot_str);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}