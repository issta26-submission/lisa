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
//<ID> 1908
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

    // step 2: Configure
    cJSON *name_added = cJSON_AddStringToObject(settings, "name", "example");
    cJSON *active_added = cJSON_AddFalseToObject(settings, "active");

    // step 3: Operate and Validate
    cJSON_bool has_settings = cJSON_HasObjectItem(root, "settings");
    cJSON *active_item = cJSON_GetObjectItem(settings, "active");
    cJSON_bool active_is_bool = cJSON_IsBool(active_item);
    cJSON *name_item = cJSON_GetObjectItem(settings, "name");
    char *name_str = cJSON_GetStringValue(name_item);
    char *printed = cJSON_PrintUnformatted(root);
    char *buffer = (char *)cJSON_malloc(64);
    memset(buffer, 0, 64);
    buffer[0] = printed ? printed[0] : 'X';
    buffer[1] = (char)('0' + (has_settings % 10));
    buffer[2] = (char)('0' + (active_is_bool % 10));
    buffer[3] = name_str ? name_str[0] : 'X';
    buffer[4] = '\0';

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}