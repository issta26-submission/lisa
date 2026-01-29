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
//<ID> 424
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
    cJSON *title = cJSON_CreateString("demo");
    cJSON_AddItemToObject(root, "title", title);
    cJSON_AddItemToObject(root, "config", config);

    // step 2: Configure
    cJSON *thr_item = cJSON_AddNumberToObject(config, "threshold", 3.1415);
    cJSON *ver_item = cJSON_AddNumberToObject(root, "version", 2.0);
    cJSON *mode = cJSON_CreateString("alpha");
    cJSON_AddItemToObject(config, "mode", mode);

    // step 3: Operate
    cJSON_bool root_is_obj = cJSON_IsObject(root);
    cJSON_bool config_is_obj = cJSON_IsObject(config);
    double threshold = cJSON_GetNumberValue(cJSON_GetObjectItem(config, "threshold"));
    double version = cJSON_GetNumberValue(cJSON_GetObjectItem(root, "version"));
    double result = threshold * version + 1.0;
    cJSON_AddNumberToObject(root, "computed_result", result);

    // step 4: Validate and Cleanup
    double retrieved = cJSON_GetNumberValue(cJSON_GetObjectItem(root, "computed_result"));
    (void)root_is_obj;
    (void)config_is_obj;
    (void)retrieved;
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}