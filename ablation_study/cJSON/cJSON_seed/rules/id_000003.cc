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
//<ID> 3
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations and initialization
    cJSON *root = cJSON_CreateObject();
    cJSON *item_true = cJSON_CreateTrue();
    cJSON *item_raw = cJSON_CreateRaw("raw_payload");
    char scratch[64];
    memset(scratch, 0, sizeof(scratch));

    // step 2: Configure JSON structure
    cJSON_AddItemToObject(root, "enabled", item_true);
    cJSON_AddItemToObject(root, "payload", item_raw);
    cJSON_AddNullToObject(root, "missing");

    // step 3: Operate and validate (no branches)
    cJSON *got_enabled = cJSON_GetObjectItem(root, "enabled");
    cJSON *got_payload = cJSON_GetObjectItem(root, "payload");
    cJSON *got_missing = cJSON_GetObjectItem(root, "missing");
    cJSON_bool enabled_is_true = cJSON_IsTrue(got_enabled);
    cJSON_bool payload_is_raw = cJSON_IsRaw(got_payload);
    scratch[0] = enabled_is_true ? 'T' : 'F';
    scratch[1] = payload_is_raw ? 'R' : 'r';
    scratch[2] = got_missing ? 'N' : 'n';
    char *out = cJSON_PrintUnformatted(root);
    scratch[3] = out ? out[0] : 0;
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);
    memset(scratch, 0, sizeof(scratch));
    // API sequence test completed successfully
    return 66;
}