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
//<ID> 1497
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *config = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "config", config);
    cJSON *name = cJSON_CreateString("initial");
    cJSON_AddItemToObject(config, "name", name);
    cJSON_AddBoolToObject(root, "enabled", 1);

    // step 2: Configure
    char prev_c = (cJSON_GetStringValue(name) ? cJSON_GetStringValue(name)[0] : 'i');
    cJSON_SetValuestring(name, "updated");
    char curr_c = (cJSON_GetStringValue(name) ? cJSON_GetStringValue(name)[0] : 'u');

    // step 3: Operate and Validate
    char *out = cJSON_PrintUnformatted(root);
    size_t out_len = out ? strlen(out) : (size_t)0;
    char *scratch = (char *)cJSON_malloc(out_len + 16);
    memset(scratch, 0, out_len + 16);
    scratch[0] = prev_c;
    scratch[1] = curr_c;
    scratch[2] = out_len ? out[0] : ' ';
    scratch[3] = (char)('0' + (int)cJSON_IsTrue(cJSON_GetObjectItem(root, "enabled")));

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}