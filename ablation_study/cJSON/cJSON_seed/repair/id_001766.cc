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
//<ID> 1766
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *stats = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "stats", stats);
    cJSON *metrics = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "metrics", metrics);

    // step 2: Configure
    cJSON *num0 = cJSON_CreateNumber(0.0);
    cJSON_InsertItemInArray(metrics, 0, num0);
    double set_result = cJSON_SetNumberHelper(num0, 123.456);
    cJSON *temp_item = cJSON_CreateNumber(25.0);
    cJSON_AddItemToObject(stats, "temp", temp_item);
    cJSON *replacement_temp = cJSON_CreateNumber(30.5);
    cJSON_ReplaceItemViaPointer(stats, temp_item, replacement_temp);
    cJSON *state_str_item = cJSON_CreateString("active");
    cJSON_AddItemToObject(root, "state", state_str_item);

    // step 3: Operate and Validate
    char *printed = cJSON_PrintUnformatted(root);
    char *buffer = (char *)cJSON_malloc(128);
    memset(buffer, 0, 128);
    cJSON *first_metric = cJSON_GetArrayItem(metrics, 0);
    double extracted = cJSON_GetNumberValue(first_metric);
    char *state_val = cJSON_GetStringValue(state_str_item);
    buffer[0] = printed[0];
    buffer[1] = (char)('0' + ((int)extracted % 10));
    buffer[2] = (char)('0' + ((int)set_result % 10));
    buffer[3] = state_val[0];
    (void)buffer;
    (void)extracted;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}