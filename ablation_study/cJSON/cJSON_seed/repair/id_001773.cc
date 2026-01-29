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
//<ID> 1773
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
    cJSON_AddStringToObject(config, "name", "test-suite");
    cJSON_AddNumberToObject(config, "version", 1.23);
    cJSON *features = cJSON_CreateArray();
    cJSON_AddItemToObject(config, "features", features);
    cJSON_AddItemToArray(features, cJSON_CreateString("alpha"));
    cJSON_AddItemToArray(features, cJSON_CreateNumber(42.0));

    // step 2: Configure
    cJSON *config_copy = cJSON_Duplicate(config, 1);
    cJSON_bool same = cJSON_Compare(config, config_copy, 1);

    // step 3: Operate and Validate
    cJSON *cfg_from_root = cJSON_GetObjectItem(root, "config");
    cJSON *name_item = cJSON_GetObjectItem(cfg_from_root, "name");
    cJSON_bool name_is_string = cJSON_IsString(name_item);
    char *name_str = cJSON_GetStringValue(name_item);
    cJSON *ver_item = cJSON_GetObjectItem(cfg_from_root, "version");
    double version_value = cJSON_GetNumberValue(ver_item);
    cJSON *features_arr = cJSON_GetObjectItem(cfg_from_root, "features");
    cJSON *first_feature = cJSON_GetArrayItem(features_arr, 0);
    cJSON_bool first_is_string = cJSON_IsString(first_feature);
    char *first_str = cJSON_GetStringValue(first_feature);
    cJSON *second_feature = cJSON_GetArrayItem(features_arr, 1);
    double before_change = cJSON_GetNumberValue(second_feature);
    double after_change = cJSON_SetNumberHelper(second_feature, 100.5);
    char *printed = cJSON_PrintUnformatted(root);
    char *buffer = (char *)cJSON_malloc(128);
    memset(buffer, 0, 128);
    buffer[0] = printed[0];
    buffer[1] = name_str[0];
    buffer[2] = (char)('0' + (int)version_value);
    buffer[3] = first_str[0];
    buffer[4] = (char)('0' + (int)after_change % 10);
    buffer[5] = (char)('0' + (int)same);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(config_copy);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}