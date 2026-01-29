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
//<ID> 193
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
    cJSON *threshold = cJSON_CreateNumber(3.14);
    cJSON_AddItemToObject(settings, "threshold", threshold);
    cJSON *version = cJSON_CreateString("v1.0");
    cJSON_AddItemToObject(settings, "version", version);

    // step 2: Configure
    cJSON *new_threshold = cJSON_CreateNumber(6.28);
    cJSON_ReplaceItemInObjectCaseSensitive(settings, "threshold", new_threshold);
    cJSON *ver_item = cJSON_GetObjectItemCaseSensitive(settings, "version");
    cJSON_bool version_is_str = cJSON_IsString(ver_item);
    cJSON *th_item = cJSON_GetObjectItemCaseSensitive(settings, "threshold");
    double threshold_val = cJSON_GetNumberValue(th_item);

    // step 3: Operate and Validate
    char *out = cJSON_PrintUnformatted(root);
    cJSON_Minify(out);
    void *tmp = cJSON_malloc(128);
    memset(tmp, 0, 128);
    cJSON *parsed = cJSON_Parse(out);
    cJSON *parsed_settings = cJSON_GetObjectItemCaseSensitive(parsed, "settings");
    cJSON *parsed_thresh = cJSON_GetObjectItemCaseSensitive(parsed_settings, "threshold");
    double parsed_val = cJSON_GetNumberValue(parsed_thresh);
    cJSON *parsed_ver = cJSON_GetObjectItemCaseSensitive(parsed_settings, "version");
    cJSON_bool parsed_ver_is_str = cJSON_IsString(parsed_ver);
    int summary = (int)threshold_val + (int)parsed_val + (int)version_is_str + (int)parsed_ver_is_str + root->type + parsed->type;
    (void)summary;

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_free(tmp);
    cJSON_Delete(parsed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}