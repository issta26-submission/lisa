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
//<ID> 1701
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON_AddStringToObject(root, "name", "example-name");
    cJSON_AddNumberToObject(root, "count", 3.14);
    cJSON_AddFalseToObject(root, "available");
    cJSON *explicit_bool = cJSON_CreateBool(1);
    cJSON_AddItemToObject(root, "explicit", explicit_bool);
    float arr_vals[3] = {1.0f, 2.5f, 3.75f};
    cJSON *float_array = cJSON_CreateFloatArray(arr_vals, 3);
    cJSON_AddItemToObject(root, "floats", float_array);
    cJSON *meta = cJSON_AddObjectToObject(root, "meta");
    cJSON_AddStringToObject(meta, "note", "configured");

    // step 2: Configure
    cJSON *retr_name_item = cJSON_GetObjectItem(root, "name");
    char *name_val = cJSON_GetStringValue(retr_name_item);
    cJSON *retr_available = cJSON_GetObjectItem(root, "available");
    cJSON *retr_count_item = cJSON_GetObjectItem(root, "count");
    double count_val = cJSON_GetNumberValue(retr_count_item);

    // step 3: Operate and Validate
    char *printed = cJSON_PrintUnformatted(root);
    char buffer[64];
    memset(buffer, 0, sizeof(buffer));
    buffer[0] = printed ? printed[0] : 'X';
    buffer[1] = name_val ? name_val[0] : 'Y';
    buffer[2] = (char)('0' + (int)cJSON_IsFalse(retr_available));
    int floats_size = cJSON_GetArraySize(float_array);
    buffer[3] = (char)('0' + (floats_size % 10));
    (void)count_val;
    (void)buffer;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}