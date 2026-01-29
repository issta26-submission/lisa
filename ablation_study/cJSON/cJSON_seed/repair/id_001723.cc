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
//<ID> 1723
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
    cJSON *num_node = cJSON_CreateNumber(1.0);
    cJSON_AddItemToObject(config, "threshold", num_node);
    cJSON *flag = cJSON_CreateTrue();
    cJSON_AddItemToObject(config, "enabled", flag);
    cJSON_AddNullToObject(config, "optional");
    cJSON *values = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "values", values);
    cJSON_AddItemToArray(values, cJSON_CreateNumber(10.0));
    cJSON_AddItemToArray(values, cJSON_CreateNumber(20.0));
    cJSON_AddItemToArray(values, cJSON_CreateNumber(30.0));

    // step 2: Configure
    cJSON_SetNumberHelper(num_node, 42.42);
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddStringToObject(meta, "author", "fuzzer");
    cJSON_AddNumberToObject(meta, "revision", 3.0);

    // step 3: Operate and Validate
    cJSON *retr_flag = cJSON_GetObjectItem(config, "enabled");
    cJSON_bool is_true = cJSON_IsTrue(retr_flag);
    char *printed = cJSON_PrintUnformatted(root);
    char *buffer = (char *)cJSON_malloc(128);
    memset(buffer, 0, 128);
    buffer[0] = printed ? printed[0] : 'X';
    buffer[1] = is_true ? 'T' : 'F';
    double current_threshold = cJSON_GetNumberValue(num_node);
    int values_count = cJSON_GetArraySize(values);
    (void)current_threshold;
    (void)values_count;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}