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
//<ID> 149
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *settings = cJSON_CreateObject();
    cJSON *null_item = cJSON_CreateNull();
    cJSON *count = cJSON_CreateNumber(7.0);
    cJSON *name = cJSON_CreateString("example");
    cJSON_AddItemToObjectCS(root, "settings", settings);
    cJSON_AddItemToObjectCS(settings, "optional", null_item);
    cJSON_AddItemToObjectCS(settings, "count", count);
    cJSON_AddItemToObjectCS(root, "name", name);

    // step 2: Configure
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddStringToObject(meta, "version", "1.2");
    cJSON_AddItemToObjectCS(root, "meta", meta);
    char prebuf[256];
    memset(prebuf, 0, sizeof(prebuf));

    // step 3: Operate and Validate
    const cJSON *settings_ptr = cJSON_GetObjectItemCaseSensitive(root, "settings");
    const cJSON *optional_ptr = cJSON_GetObjectItemCaseSensitive(settings_ptr, "optional");
    cJSON_bool optional_is_null = cJSON_IsNull(optional_ptr);
    const cJSON *count_ptr = cJSON_GetObjectItemCaseSensitive(settings_ptr, "count");
    double count_val = cJSON_GetNumberValue(count_ptr);
    char *printed = cJSON_PrintBuffered(root, 256, 1);
    int summary = (int)optional_is_null + (int)count_val + (printed ? (int)printed[0] : 0) + root->type + settings->type;
    (void)summary;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}