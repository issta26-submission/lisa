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
//<ID> 1053
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json[] = "{\"items\":[{\"id\":1},{\"id\":2,\"name\":\"second\"}],\"meta\":{\"count\":2}}";
    size_t json_len = sizeof(json) - 1;
    cJSON *root = cJSON_ParseWithLength(json, json_len);

    // step 2: Configure
    cJSON *items = cJSON_GetObjectItem(root, "items");
    cJSON *second_item = cJSON_GetArrayItem(items, 1);
    cJSON_bool second_is_invalid = cJSON_IsInvalid(second_item);
    cJSON *out = cJSON_CreateObject();
    cJSON_AddBoolToObject(out, "second_invalid", second_is_invalid);
    cJSON *meta_dup = cJSON_Duplicate(cJSON_GetObjectItem(root, "meta"), 1);
    cJSON_AddItemToObject(out, "meta_copy", meta_dup);

    // step 3: Operate
    char *pretty = cJSON_Print(out);
    int buf_len = 256;
    char *prealloc_buf = (char *)cJSON_malloc((size_t)buf_len);
    memset(prealloc_buf, 0, buf_len);
    cJSON_PrintPreallocated(out, prealloc_buf, buf_len, 1);

    // step 4: Validate & Cleanup
    cJSON_free(pretty);
    cJSON_free(prealloc_buf);
    cJSON_Delete(root);
    cJSON_Delete(out);

    // API sequence test completed successfully
    return 66;
}