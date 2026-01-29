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
//<ID> 1014
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *settings = cJSON_AddObjectToObject(root, "settings");
    cJSON_AddBoolToObject(settings, "enabled", 1);
    cJSON_AddBoolToObject(root, "root_flag", 0);

    // step 2: Configure
    cJSON *network = cJSON_AddObjectToObject(settings, "network");
    cJSON_AddBoolToObject(network, "dhcp", 1);
    cJSON_AddBoolToObject(network, "wifi", 0);

    // step 3: Operate and Validate
    cJSON *wifi_item = cJSON_GetObjectItem(network, "wifi");
    cJSON_bool has_settings = cJSON_HasObjectItem(root, "settings");
    cJSON_AddBoolToObject(root, "has_settings_copy", has_settings);
    cJSON *detached_wifi = cJSON_DetachItemViaPointer(network, wifi_item);
    cJSON_Delete(detached_wifi);

    // step 4: Cleanup
    char *flat = cJSON_PrintUnformatted(root);
    cJSON_free(flat);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}