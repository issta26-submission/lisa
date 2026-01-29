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
//<ID> 162
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *values = cJSON_CreateArray();
    cJSON *true_item = cJSON_CreateTrue();
    cJSON *false_item = cJSON_CreateFalse();
    cJSON *num_item = cJSON_CreateNumber(42.0);
    cJSON_AddItemToObject(root, "values", values);
    cJSON_InsertItemInArray(values, 0, true_item);
    cJSON_InsertItemInArray(values, 1, false_item);
    cJSON_InsertItemInArray(values, 1, num_item);

    // step 2: Configure
    cJSON *config = cJSON_AddObjectToObject(root, "config");
    cJSON_AddTrueToObject(config, "enabled");
    cJSON *meta = cJSON_AddObjectToObject(root, "meta");
    cJSON_AddStringToObject(meta, "name", "example");

    // step 3: Operate and Validate
    int array_size = cJSON_GetArraySize(values);
    cJSON *first = cJSON_GetArrayItem(values, 0);
    cJSON *second = cJSON_GetArrayItem(values, 1);
    cJSON_bool first_is_bool = cJSON_IsBool(first);
    cJSON_bool second_is_bool = cJSON_IsBool(second);
    cJSON *detached = cJSON_DetachItemFromArray(values, 2);
    char *printed = cJSON_PrintUnformatted(root);
    int buf_len = 256;
    char *buf = (char *)cJSON_malloc((size_t)buf_len);
    memset(buf, 0, buf_len);
    cJSON_PrintPreallocated(root, buf, buf_len, 1);
    int summary = array_size + (int)first_is_bool + (int)second_is_bool + (printed ? (int)printed[0] : 0) + root->type + config->type;
    (void)summary;

    // step 4: Cleanup
    cJSON_free(buf);
    cJSON_free(printed);
    cJSON_Delete(detached);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}