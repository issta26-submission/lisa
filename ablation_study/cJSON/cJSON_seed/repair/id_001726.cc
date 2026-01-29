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
//<ID> 1726
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
    cJSON *num_node = cJSON_CreateNumber(0.0);
    cJSON_AddItemToObject(config, "threshold", num_node);
    double set_number_result = cJSON_SetNumberHelper(num_node, 42.75);
    (void)set_number_result;
    cJSON_AddNullToObject(root, "optional");
    cJSON *enabled_node = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "enabled", enabled_node);

    // step 2: Configure
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddStringToObject(meta, "owner", "tester");
    cJSON_AddNumberToObject(meta, "version", 1.2);

    // step 3: Operate and Validate
    cJSON_bool enabled_flag = cJSON_IsTrue(enabled_node);
    char *printed = cJSON_PrintUnformatted(root);
    char *buffer = (char *)cJSON_malloc(64);
    memset(buffer, 0, 64);
    buffer[0] = printed[0];
    buffer[1] = (char)('0' + (int)enabled_flag);
    cJSON *retr_config = cJSON_GetObjectItem(root, "config");
    cJSON *retr_threshold = cJSON_GetObjectItem(retr_config, "threshold");
    double threshold_val = cJSON_GetNumberValue(retr_threshold);
    buffer[2] = (char)('0' + ((int)threshold_val % 10));
    (void)buffer;
    (void)threshold_val;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}