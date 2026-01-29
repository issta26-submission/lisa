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
//<ID> 485
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json[] = "{\"name\":\"alice\",\"active\":false}";
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithLengthOpts(json, sizeof(json) - 1, &parse_end, 1);
    cJSON *name_item = cJSON_GetObjectItem(root, "name");
    char *name_str = cJSON_GetStringValue(name_item);
    cJSON *active_item = cJSON_GetObjectItem(root, "active");
    cJSON_bool name_was_string = cJSON_IsString(name_item);

    // step 2: Configure
    cJSON *new_active = cJSON_CreateBool(1);
    cJSON_ReplaceItemInObject(root, "active", new_active);
    size_t name_len = (name_str != NULL) ? (strlen(name_str) + 1) : 1;
    char *tmp_buf = (char *)cJSON_malloc(name_len);
    memset(tmp_buf, 0, name_len);
    memcpy(tmp_buf, name_str, name_len - 1);
    cJSON *name_copy = cJSON_CreateString(tmp_buf);
    cJSON_AddItemToObject(root, "name_copy", name_copy);

    // step 3: Operate
    cJSON_bool copy_is_string = cJSON_IsString(name_copy);
    char *snapshot = cJSON_PrintUnformatted(root);

    // step 4: Validate & Cleanup
    (void)name_was_string;
    (void)copy_is_string;
    cJSON_free(tmp_buf);
    cJSON_free(snapshot);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}