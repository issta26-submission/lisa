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
//<ID> 972
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *config = cJSON_AddObjectToObject(root, "config");
    cJSON *temp = cJSON_AddObjectToObject(root, "temp");

    // step 2: Configure
    cJSON *port = cJSON_AddNumberToObject(config, "port", 8080.0);
    cJSON *host = cJSON_AddStringToObject(config, "host", "localhost");
    cJSON *tmp_val = cJSON_AddStringToObject(temp, "note", "transient");

    // step 3: Operate and Validate
    char buffer[256];
    memset(buffer, 0, sizeof(buffer));
    cJSON_bool printed_ok = cJSON_PrintPreallocated(root, buffer, (int)sizeof(buffer), 1);
    cJSON *detached_temp = cJSON_DetachItemFromObject(root, "temp");
    cJSON_bool readded_ok = cJSON_AddItemToObjectCS(root, "temp", detached_temp);
    cJSON *cfg_lookup = cJSON_GetObjectItem(root, "config");
    cJSON *port_item = cJSON_GetObjectItem(cfg_lookup, "port");
    double port_value = cJSON_GetNumberValue(port_item);
    cJSON *host_item = cJSON_GetObjectItem(cfg_lookup, "host");
    const char *host_value = cJSON_GetStringValue(host_item);
    memset(buffer, 0, sizeof(buffer));
    (void)printed_ok;
    (void)readded_ok;
    (void)port_value;
    (void)host_value;

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}