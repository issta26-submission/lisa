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
//<ID> 1433
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json_text = "{\"name\":\"alpha\",\"data\":{\"a\":1,\"b\":2}}";
    size_t json_len = strlen(json_text);
    cJSON *root = cJSON_ParseWithLength(json_text, json_len);
    cJSON *data = cJSON_GetObjectItem(root, "data");
    cJSON *name_item = cJSON_GetObjectItem(root, "name");
    const char *name_str = cJSON_GetStringValue(name_item);

    // step 2: Configure
    cJSON *dup_data = cJSON_Duplicate(data, 1);
    cJSON_AddItemToObject(root, "data_copy", dup_data);
    cJSON *raw_item = cJSON_AddRawToObject(root, "raw_injection", "{\"in\":true}");
    cJSON *detached_raw = cJSON_DetachItemViaPointer(root, raw_item);
    cJSON_AddItemToObject(dup_data, "moved_raw", detached_raw);

    // step 3: Operate & Validate
    int buf_len = 512;
    char *buffer = (char *)cJSON_malloc(buf_len);
    memset(buffer, 0, buf_len);
    cJSON_PrintPreallocated(root, buffer, buf_len, 1);
    size_t printed_len = strlen(buffer);
    (void)name_str;
    (void)printed_len;

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}