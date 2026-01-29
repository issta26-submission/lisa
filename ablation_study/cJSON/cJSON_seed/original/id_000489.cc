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
//<ID> 489
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json[] = "{\"name\":\"Alice\",\"age\":30,\"active\":false,\"notes\":null}";
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithLengthOpts(json, sizeof(json) - 1, &parse_end, 1);
    cJSON *name_item = cJSON_GetObjectItem(root, "name");
    cJSON *age_item = cJSON_GetObjectItem(root, "age");
    double age_val = cJSON_GetNumberValue(age_item);

    // step 2: Configure
    cJSON *new_active = cJSON_CreateBool(1);
    cJSON_ReplaceItemInObject(root, "active", new_active);
    cJSON_bool name_is_string = cJSON_IsString(name_item);
    cJSON *age_copy = cJSON_CreateNumber(age_val);
    cJSON_AddItemToObject(root, "age_copy", age_copy);

    // step 3: Operate
    size_t buf_len = (size_t)64 + (size_t)32 * (size_t)name_is_string;
    void *buffer = cJSON_malloc(buf_len);
    memset(buffer, 0, buf_len);
    char *snapshot = cJSON_PrintUnformatted(root);
    char *name_str = cJSON_GetStringValue(name_item);
    cJSON *owner_ref = cJSON_CreateStringReference(name_str);
    cJSON_AddItemToObject(root, "owner", owner_ref);

    // step 4: Validate & Cleanup
    cJSON_free(snapshot);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}