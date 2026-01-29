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
//<ID> 1197
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddStringToObject(root, "title", "example");
    cJSON_AddNullToObject(root, "optional");
    cJSON *num = cJSON_CreateNumber(5.0);
    cJSON_AddItemToObject(meta, "num", num);
    cJSON_SetNumberHelper(num, 42.5);

    const char *raw = "{\"external\":10,\"keep\":\"yes\"}";
    const char *endptr = NULL;
    cJSON *parsed = cJSON_ParseWithLengthOpts(raw, strlen(raw), &endptr, 1);
    cJSON_AddItemToObject(root, "parsed", parsed);

    // step 2: Configure
    cJSON *keep_item = cJSON_GetObjectItem(parsed, "keep");
    cJSON *detached = cJSON_DetachItemViaPointer(parsed, keep_item);
    cJSON_AddItemToObject(root, "kept", detached);

    char *out = cJSON_PrintUnformatted(root);
    void *scratch = cJSON_malloc(128);
    memset(scratch, 0, 128);
    ((char *)scratch)[0] = out[0];
    ((char *)scratch)[1] = out[1];

    // step 3: Operate and Validate
    double external = cJSON_GetNumberValue(cJSON_GetObjectItem(parsed, "external"));
    double internal = cJSON_GetNumberValue(cJSON_GetObjectItem(meta, "num"));
    double combined = external + internal;
    cJSON_AddNumberToObject(root, "combined", combined);
    char *title = cJSON_GetStringValue(cJSON_GetObjectItem(root, "title"));
    (void)title;
    cJSON_bool was_null = cJSON_IsNull(cJSON_GetObjectItem(root, "optional"));
    (void)was_null;

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_free(scratch);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}