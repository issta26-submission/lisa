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
//<ID> 827
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
    cJSON *threshold = cJSON_AddNumberToObject(settings, "threshold", 42.5);
    cJSON *label = cJSON_CreateString("primary");
    cJSON_AddItemToObject(settings, "label", label);
    cJSON *count_num = cJSON_CreateNumber(100.0);
    cJSON_AddItemToObject(root, "count", count_num);

    // step 2: Configure
    const char *json_text = "{\"external\":\"value\",\"settings\":{\"max\":99.9}}";
    cJSON *parsed = cJSON_Parse(json_text);
    cJSON *parsed_settings = cJSON_GetObjectItem(parsed, "settings");
    cJSON *parsed_max_item = cJSON_GetObjectItem(parsed_settings, "max");
    double parsed_max = cJSON_GetNumberValue(parsed_max_item);

    // step 3: Operate and Validate
    cJSON *detached_threshold = cJSON_DetachItemViaPointer(settings, threshold);
    cJSON_AddItemToObject(root, "detached_threshold", detached_threshold);
    cJSON *detached_count = cJSON_DetachItemFromObject(root, "count");
    cJSON_AddItemToObject(settings, "moved_count", detached_count);
    cJSON *ext_item = cJSON_GetObjectItem(parsed, "external");
    cJSON_bool ext_is_string = cJSON_IsString(ext_item);
    cJSON_bool label_is_string = cJSON_IsString(label);
    double thr_val = cJSON_GetNumberValue(detached_threshold);
    double moved_count_val = cJSON_GetNumberValue(cJSON_GetObjectItem(settings, "moved_count"));
    (void)parsed_max;
    (void)thr_val;
    (void)moved_count_val;
    (void)ext_is_string;
    (void)label_is_string;

    // step 4: Cleanup
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}