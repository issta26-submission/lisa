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
//<ID> 1439
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json_text[] = "{\"obj\":{\"a\":1,\"b\":[10,20]},\"status\":\"ok\"}";
    cJSON *root = cJSON_ParseWithLength(json_text, sizeof(json_text) - 1);
    cJSON *obj = cJSON_GetObjectItem(root, "obj");

    // step 2: Configure
    cJSON *dup_obj = cJSON_Duplicate(obj, 1);
    cJSON *detached_obj = cJSON_DetachItemViaPointer(root, obj);
    cJSON *raw_added = cJSON_AddRawToObject(root, "injected", "{\"rawnum\":123}");

    // step 3: Operate & Validate
    int buf_len = 512;
    char *buffer = (char *)cJSON_malloc((size_t)buf_len);
    memset(buffer, 0, (size_t)buf_len);
    cJSON_PrintPreallocated(root, buffer, buf_len, 0);
    cJSON *dup_a = cJSON_GetObjectItem(dup_obj, "a");
    double dup_a_val = cJSON_GetNumberValue(dup_a);
    cJSON *inj = cJSON_GetObjectItem(root, "injected");
    const char *inj_raw = cJSON_GetStringValue(inj);
    (void)dup_a_val;
    (void)inj_raw;

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(dup_obj);
    cJSON_Delete(detached_obj);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}