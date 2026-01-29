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
//<ID> 571
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *info = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "info", info);
    cJSON *name = cJSON_CreateString("initial");
    cJSON_AddItemToObject(info, "name", name);
    cJSON *count = cJSON_CreateNumber(10.5);
    cJSON_AddItemToObject(info, "count", count);
    cJSON *enabled = cJSON_CreateFalse();
    cJSON_AddItemToObject(info, "enabled", enabled);

    // step 2: Configure
    char *new_name = cJSON_SetValuestring(name, "renamed");
    (void)new_name;
    cJSON *extra = cJSON_CreateNumber(2.25);
    cJSON_AddItemToObject(root, "extra", extra);

    // step 3: Operate and Validate
    cJSON *got_name = cJSON_GetObjectItemCaseSensitive(info, "name");
    char *fetched_name = cJSON_GetStringValue(got_name);
    cJSON *got_count = cJSON_GetObjectItemCaseSensitive(info, "count");
    double fetched_count = cJSON_GetNumberValue(got_count);
    cJSON *got_enabled = cJSON_GetObjectItemCaseSensitive(info, "enabled");
    cJSON_bool was_false = cJSON_IsFalse(got_enabled);
    double adjusted = fetched_count + ((1.0 - (double)was_false) * 1.0);
    cJSON *result = cJSON_CreateNumber(adjusted);
    cJSON_AddItemToObject(root, "result", result);
    cJSON_AddStringToObject(root, "confirmed_name", fetched_name);

    // step 4: Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}