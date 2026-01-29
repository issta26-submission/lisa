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
//<ID> 141
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *items = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", items);
    cJSON *meta = cJSON_AddObjectToObject(root, "meta");

    // step 2: Configure
    cJSON_AddStringToObject(meta, "version", cJSON_Version());
    cJSON_AddStringToObject(root, "source", "generated");
    cJSON *ttrue = cJSON_CreateTrue();
    cJSON_AddItemToArray(items, ttrue);
    cJSON *n1 = cJSON_CreateNumber(42.0);
    cJSON_AddItemToArray(items, n1);

    // step 3: Operate and Validate
    cJSON *parsed = cJSON_Parse("{\"copy\":\"from_parse\",\"flag\":true}");
    cJSON *copy_item = cJSON_GetObjectItem(parsed, "copy");
    const char *copy_str = cJSON_GetStringValue(copy_item);
    cJSON_AddStringToObject(root, "copy", copy_str);
    cJSON *meta_dup = cJSON_Duplicate(meta, 1);
    cJSON_AddItemToObject(root, "meta_copy", meta_dup);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}