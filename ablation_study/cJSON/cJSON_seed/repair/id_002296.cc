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
//<ID> 2296
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *items = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", items);
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);

    // step 2: Configure
    cJSON *flag_false = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "enabled", flag_false);
    cJSON_AddStringToObject(meta, "name", "device-alpha");
    cJSON *elem1 = cJSON_CreateObject();
    cJSON_AddNumberToObject(elem1, "id", 1.0);
    cJSON_AddStringToObject(elem1, "type", "sensor");
    cJSON_AddItemToArray(items, elem1);
    cJSON *elem2 = cJSON_CreateObject();
    cJSON_AddNumberToObject(elem2, "id", 2.0);
    cJSON_AddStringToObject(elem2, "type", "actuator");
    cJSON_AddItemToArray(items, elem2);

    // step 3: Operate
    cJSON_bool has_enabled = cJSON_HasObjectItem(root, "enabled");
    cJSON *enabled_present = cJSON_CreateNumber((double)has_enabled);
    cJSON_AddItemToObject(root, "enabled_present", enabled_present);
    char *json = cJSON_PrintUnformatted(root);

    // step 4: Validate and Cleanup
    const int buflen = 256;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = (json && json[0]) ? json[0] : '\0';
    cJSON_free(json);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}