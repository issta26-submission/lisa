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
//<ID> 480
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json[] = "{\"name\":\"widget\",\"count\":5,\"meta\":{\"active\":false}}";
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithLengthOpts(json, sizeof(json) - 1, &parse_end, 1);
    cJSON *name_item = cJSON_GetObjectItem(root, "name");
    cJSON_bool name_is_string = cJSON_IsString(name_item);
    cJSON *count_item = cJSON_GetObjectItem(root, "count");
    double count_val = cJSON_GetNumberValue(count_item);

    // step 2: Configure
    cJSON *new_active = cJSON_CreateBool(name_is_string);
    cJSON *meta = cJSON_GetObjectItem(root, "meta");
    cJSON_ReplaceItemInObject(meta, "active", new_active);
    double new_count_val = count_val + (double)name_is_string;
    cJSON *new_count = cJSON_CreateNumber(new_count_val);
    cJSON_ReplaceItemInObject(root, "count", new_count);

    // step 3: Operate
    char *buffer = (char *)cJSON_malloc(256);
    cJSON_PrintPreallocated(root, buffer, 256, 1);
    cJSON *name_ref = cJSON_GetObjectItem(root, "name");
    char *name_str = cJSON_GetStringValue(name_ref);
    cJSON *flag_item = cJSON_CreateBool(cJSON_IsString(name_ref));
    cJSON_AddItemToObject(root, "name_is_string", flag_item);

    // step 4: Validate & Cleanup
    (void)name_str;
    (void)new_count_val;
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}