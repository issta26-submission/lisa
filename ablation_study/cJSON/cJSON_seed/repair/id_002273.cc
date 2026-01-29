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
//<ID> 2273
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *payload = cJSON_CreateRaw("{\"inner\":123}");
    cJSON_AddItemToObject(root, "payload", payload);
    cJSON *status = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "status", status);
    cJSON *info = cJSON_CreateObject();
    cJSON *info_flag = cJSON_CreateTrue();
    cJSON_AddItemToObject(info, "flag", info_flag);
    cJSON_AddItemToObject(root, "info", info);

    // step 2: Configure
    cJSON *count = cJSON_CreateNumber(7.0);
    cJSON_AddItemToObject(root, "count", count);
    cJSON *note = cJSON_CreateString("example");
    cJSON_AddItemToObject(root, "note", note);

    // step 3: Operate and Validate
    cJSON *got_payload = cJSON_GetObjectItem(root, "payload");
    cJSON_bool payload_is_raw = cJSON_IsRaw(got_payload);
    cJSON *got_status = cJSON_GetObjectItem(root, "status");
    cJSON *replicated = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "replicated", replicated);
    const int buflen = 256;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    cJSON_bool printed = cJSON_PrintPreallocated(root, buffer, buflen, 1);
    buffer[0] = (char)('0' + (int)payload_is_raw);
    buffer[1] = (char)('0' + (int)printed);

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}