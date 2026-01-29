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
//<ID> 483
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json[] = "{\"name\":\"Alice\",\"active\":false,\"meta\":{\"id\":42}}";
    const char *parse_end = NULL;
    size_t json_len = (size_t)(sizeof(json) - 1);
    cJSON *root = cJSON_ParseWithLengthOpts(json, json_len, &parse_end, 1);
    cJSON *name_item = cJSON_GetObjectItem(root, "name");
    const char *name_str = cJSON_GetStringValue(name_item);
    cJSON_bool name_was_string = cJSON_IsString(name_item);

    // step 2: Configure
    cJSON *new_active = cJSON_CreateBool(1);
    cJSON_bool replaced_ok = cJSON_ReplaceItemInObject(root, "active", new_active);

    // step 3: Operate
    int buf_size = 256;
    char *print_buffer = (char *)cJSON_malloc((size_t)buf_size);
    memset(print_buffer, 0, (size_t)buf_size);
    cJSON_PrintPreallocated(root, print_buffer, buf_size, 0);
    double meta_id = cJSON_GetNumberValue(cJSON_GetObjectItem(cJSON_GetObjectItem(root, "meta"), "id"));

    // step 4: Validate & Cleanup
    (void)name_str;
    (void)name_was_string;
    (void)replaced_ok;
    (void)meta_id;
    cJSON_free(print_buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}