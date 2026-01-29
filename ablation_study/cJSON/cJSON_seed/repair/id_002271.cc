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
//<ID> 2271
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
    cJSON *status = cJSON_CreateTrue();
    cJSON_AddItemToObject(meta, "status", status);
    cJSON *name = cJSON_CreateString("sensor-007");
    cJSON_AddItemToObject(meta, "name", name);
    cJSON *payload = cJSON_CreateRaw("{\"a\":1}");
    cJSON_AddItemToObject(root, "payload", payload);

    // step 2: Configure
    const int buflen = 256;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    cJSON_bool printed = cJSON_PrintPreallocated(root, buffer, buflen, 1);
    buffer[0] = (char)('0' + (int)printed);

    // step 3: Operate and Validate
    cJSON *got_payload = cJSON_GetObjectItem(root, "payload");
    cJSON_bool raw_flag = cJSON_IsRaw(got_payload);
    cJSON *raw_bool = cJSON_CreateBool(raw_flag);
    cJSON_AddItemToObject(root, "payload_is_raw", raw_bool);
    cJSON *verify = cJSON_GetObjectItem(root, "payload_is_raw");
    buffer[1] = (char)('A' + (verify->type & 0x0F));

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}