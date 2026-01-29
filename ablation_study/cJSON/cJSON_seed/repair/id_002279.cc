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
//<ID> 2279
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
    cJSON *active_true = cJSON_CreateTrue();
    cJSON_AddItemToObject(meta, "active", active_true);
    cJSON *payload = cJSON_CreateRaw("{\"id\":123,\"type\":\"raw\"}");
    cJSON_AddItemToObject(root, "payload", payload);
    cJSON *info = cJSON_CreateObject();
    cJSON_AddStringToObject(info, "name", "sensor-X");
    cJSON_AddItemToObject(root, "info", info);

    // step 2: Configure
    cJSON *diagnostics = cJSON_CreateObject();
    cJSON_AddNumberToObject(diagnostics, "uptime", 1024.0);
    cJSON_AddItemToObject(root, "diagnostics", diagnostics);
    cJSON *ref_true = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "flag_ref", ref_true);

    // step 3: Operate
    cJSON *got_payload = cJSON_GetObjectItem(root, "payload");
    cJSON_bool payload_is_raw = cJSON_IsRaw(got_payload);
    cJSON *raw_flag_num = cJSON_CreateNumber((double)payload_is_raw);
    cJSON_AddItemToObject(root, "payload_is_raw", raw_flag_num);

    // step 4: Validate and Cleanup
    const int buflen = 512;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    cJSON_bool printed = cJSON_PrintPreallocated(root, buffer, buflen, 1);
    (void)printed; /* use printed to avoid unused-variable warnings */
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}