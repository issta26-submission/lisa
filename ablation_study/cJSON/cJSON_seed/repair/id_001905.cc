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
//<ID> 1905
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
    cJSON *name_item = cJSON_AddStringToObject(settings, "name", "example");
    cJSON *disabled_item = cJSON_AddFalseToObject(settings, "disabled");

    // step 2: Configure
    cJSON_bool has_settings = cJSON_HasObjectItem(root, "settings");
    cJSON *got_name = cJSON_GetObjectItem(settings, "name");
    cJSON_bool name_exists = cJSON_HasObjectItem(settings, "name");
    cJSON *got_disabled = cJSON_GetObjectItem(settings, "disabled");
    cJSON_bool disabled_is_bool = cJSON_IsBool(got_disabled);

    // step 3: Operate and Validate
    char *printed = cJSON_PrintUnformatted(root);
    char *buffer = (char *)cJSON_malloc(128);
    memset(buffer, 0, 128);
    buffer[0] = printed ? printed[0] : 'X';
    buffer[1] = (char)('0' + (int)(has_settings % 10));
    buffer[2] = (char)('0' + (int)(name_exists % 10));
    buffer[3] = (char)('0' + (int)(disabled_is_bool % 10));
    buffer[4] = '\0';

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}