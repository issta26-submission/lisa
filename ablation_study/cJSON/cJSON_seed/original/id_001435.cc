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
//<ID> 1435
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json_text = "{\"numbers\":[1,2,3],\"meta\":{\"id\":42,\"tag\":\"x\"},\"note\":\"hello\"}";
    size_t json_len = strlen(json_text);
    cJSON *parsed = cJSON_ParseWithLength(json_text, json_len);

    // step 2: Configure
    cJSON *meta_ptr = cJSON_GetObjectItem(parsed, "meta");
    cJSON *detached_meta = cJSON_DetachItemViaPointer(parsed, meta_ptr);
    cJSON *root = cJSON_CreateObject();
    cJSON *meta_dup = cJSON_Duplicate(detached_meta, 1);
    cJSON_AddItemToObject(root, "meta_original", detached_meta);
    cJSON_AddItemToObject(root, "meta_duplicate", meta_dup);
    cJSON_AddRawToObject(root, "extra_raw", "{\"inlined\":true}");

    // step 3: Operate & Validate
    char *unformatted = cJSON_PrintUnformatted(root);
    int buf_len = (int)(strlen(unformatted) + 1);
    char *buf = (char *)cJSON_malloc((size_t)buf_len);
    memset(buf, 0, buf_len);
    cJSON_bool printed_ok = cJSON_PrintPreallocated(root, buf, buf_len, 0);
    (void)printed_ok;
    cJSON_free(unformatted);

    // step 4: Cleanup
    cJSON_free(buf);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}