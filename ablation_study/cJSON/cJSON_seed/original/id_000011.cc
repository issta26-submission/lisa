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
//<ID> 11
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json = "{\"name\":\"fuzz\",\"active\":true,\"count\":3}";
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithOpts(json, &parse_end, 1);
    cJSON *dup_root = cJSON_Duplicate(root, 1);

    // step 2: Configure
    cJSON *wrapper = cJSON_CreateObject();
    cJSON_AddTrueToObject(wrapper, "wrapped_true");
    cJSON_AddFalseToObject(wrapper, "wrapped_false");
    cJSON_AddItemToObject(wrapper, "original", root);
    cJSON_AddItemToObject(wrapper, "duplicate", dup_root);

    // step 3: Operate and Validate
    char *unformatted = cJSON_PrintUnformatted(wrapper);
    cJSON *got_orig = cJSON_GetObjectItemCaseSensitive(wrapper, "original");
    cJSON *name_item = cJSON_GetObjectItem(got_orig, "name");
    const char *name_value = cJSON_GetStringValue(name_item);
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddStringToObject(meta, "original_name", name_value);
    cJSON_AddItemToObject(wrapper, "meta", meta);

    // step 4: Cleanup
    cJSON_free(unformatted);
    cJSON_Delete(wrapper);

    // API sequence test completed successfully
    return 66;
}