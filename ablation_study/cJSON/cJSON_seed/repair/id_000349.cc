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
//<ID> 349
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
    cJSON *threshold = cJSON_CreateNumber(100.0);
    cJSON *label = cJSON_CreateString("example");
    cJSON *flag = cJSON_CreateTrue();

    // step 2: Configure
    cJSON_AddItemToObject(config, "threshold", threshold);
    cJSON_AddItemToObject(config, "label", label);
    cJSON_AddItemToObject(config, "flag", flag);
    cJSON_AddItemToObject(root, "config", config);

    // step 3: Operate and Validate
    cJSON *got_label_item = cJSON_GetObjectItem(config, "label");
    char *label_str = cJSON_GetStringValue(got_label_item);
    cJSON *got_threshold_item = cJSON_GetObjectItem(config, "threshold");
    double th_value = cJSON_GetNumberValue(got_threshold_item);
    cJSON_bool flag_true = cJSON_IsTrue(cJSON_GetObjectItem(config, "flag"));
    double computed = th_value + (double)flag_true * 1.5;
    cJSON *result = cJSON_CreateNumber(computed);
    cJSON_AddItemToObject(root, "result", result);
    cJSON *label_copy = cJSON_CreateString(label_str);
    cJSON_AddItemToObject(root, "label_copy", label_copy);
    cJSON *validated_flag = cJSON_CreateNumber((double)flag_true);
    cJSON_AddItemToObject(root, "validated_flag", validated_flag);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}