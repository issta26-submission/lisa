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
//<ID> 1769
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *metrics = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "metrics", metrics);
    cJSON *values = cJSON_CreateArray();
    cJSON_AddItemToObject(metrics, "values", values);
    cJSON *v0 = cJSON_CreateNumber(10.0);
    cJSON_AddItemToArray(values, v0);
    cJSON *v2 = cJSON_CreateNumber(30.0);
    cJSON_AddItemToArray(values, v2);

    // step 2: Configure
    cJSON *v1 = cJSON_CreateNumber(20.0);
    cJSON_InsertItemInArray(values, 1, v1);
    double adjusted = cJSON_SetNumberHelper(v2, 35.5);
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON *source = cJSON_CreateString("sensor-A");
    cJSON_AddItemToObject(meta, "source", source);

    // step 3: Operate and Validate
    cJSON *new_v1 = cJSON_CreateNumber(25.0);
    cJSON_ReplaceItemViaPointer(values, v1, new_v1);
    char *printed = cJSON_PrintUnformatted(root);
    char *buffer = (char *)cJSON_malloc(64);
    memset(buffer, 0, 64);
    const char *source_str = cJSON_GetStringValue(cJSON_GetObjectItem(meta, "source"));
    buffer[0] = printed ? printed[0] : 'X';
    buffer[1] = (char)('0' + (int)cJSON_GetArraySize(values));
    buffer[2] = (char)('0' + ((int)adjusted % 10));
    buffer[3] = source_str ? source_str[0] : 'Z';
    (void)buffer;
    (void)source_str;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}