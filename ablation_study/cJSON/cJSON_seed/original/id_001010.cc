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
//<ID> 1010
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char initial_json[] = "{\"name\":\"tester\",\"active\":null,\"count\":3}";
    size_t initial_len = sizeof(initial_json) - 1;
    cJSON *root = cJSON_ParseWithLength(initial_json, initial_len);

    // step 2: Configure
    cJSON *name_item = cJSON_GetObjectItem(root, "name");
    cJSON *active_item = cJSON_GetObjectItem(root, "active");
    cJSON_bool active_is_null = cJSON_IsNull(active_item);
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddBoolToObject(meta, "was_null", active_is_null);
    cJSON *tag = cJSON_CreateString("generated");
    cJSON_AddItemToObject(meta, "tag", tag);
    cJSON_AddItemToObject(root, "meta", meta);

    // step 3: Operate
    const char *name_val = cJSON_GetStringValue(name_item);
    cJSON_AddStringToObject(root, "extracted_name", name_val);
    char *out = cJSON_PrintUnformatted(root);
    char *print_buf = (char *)cJSON_malloc(256);
    memset(print_buf, 0, 256);
    cJSON_PrintPreallocated(root, print_buf, 256, 1);

    // step 4: Validate & Cleanup
    cJSON_DeleteItemFromObject(root, "active");
    cJSON_DeleteItemFromObject(root, "count");
    cJSON_free(out);
    cJSON_free(print_buf);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}