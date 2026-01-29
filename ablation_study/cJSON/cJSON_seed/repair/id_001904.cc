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
//<ID> 1904
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
    cJSON *false_item = cJSON_AddFalseToObject(settings, "feature_enabled");
    cJSON *label_item = cJSON_AddStringToObject(settings, "label", "alpha");
    cJSON_AddStringToObject(root, "version", "1.0");

    // step 3: Operate and Validate
    cJSON_bool has_settings = cJSON_HasObjectItem(root, "settings");
    cJSON *got_false = cJSON_GetObjectItem(settings, "feature_enabled");
    cJSON_bool is_bool = cJSON_IsBool(got_false);
    const char *label_str = cJSON_GetStringValue(label_item);
    char *printed = cJSON_PrintUnformatted(root);
    char *buffer = (char *)cJSON_malloc(64);
    memset(buffer, 0, 64);
    buffer[0] = printed ? printed[0] : 'X';
    buffer[1] = (char)('0' + (int)(has_settings % 10));
    buffer[2] = (char)('0' + (int)(is_bool % 10));
    buffer[3] = label_str ? label_str[0] : 'Y';
    buffer[4] = '\0';

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}