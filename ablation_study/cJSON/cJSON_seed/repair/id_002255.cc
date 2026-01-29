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
//<ID> 2255
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *settings = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "settings", settings);
    cJSON *enabled = cJSON_AddTrueToObject(settings, "enabled");

    // step 2: Configure
    cJSON *settings_ref = cJSON_CreateObjectReference(settings);
    cJSON_AddItemToObject(root, "settings_ref", settings_ref);

    // step 3: Operate and Validate
    cJSON *got_settings = cJSON_GetObjectItem(root, "settings");
    cJSON *got_enabled = cJSON_GetObjectItem(got_settings, "enabled");
    cJSON_bool enabled_is_true = cJSON_IsTrue(got_enabled);
    cJSON *got_ref = cJSON_GetObjectItem(root, "settings_ref");
    const int buflen = 256;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    cJSON_bool printed = cJSON_PrintPreallocated(root, buffer, buflen, 1);
    buffer[0] = (char)('0' + (int)enabled_is_true);
    buffer[1] = (char)(printed ? 'P' : 'N');
    buffer[2] = '\0';

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}