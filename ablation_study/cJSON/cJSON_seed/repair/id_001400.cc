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
//<ID> 1400
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *data = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "data", data);
    cJSON_AddNumberToObject(data, "value", 42.5);
    cJSON_AddTrueToObject(data, "valid");
    cJSON *arr = cJSON_CreateArray();
    cJSON_AddItemToObject(data, "items", arr);
    cJSON *initial_num = cJSON_CreateNumber(1.0);
    cJSON_AddItemToArray(arr, initial_num);

    // step 2: Configure
    cJSON *value_item = cJSON_GetObjectItem(data, "value");
    cJSON *dup_value = cJSON_Duplicate(value_item, 0);
    cJSON_ReplaceItemInArray(arr, 0, dup_value);

    // step 3: Operate and Validate
    cJSON *first = cJSON_GetArrayItem(arr, 0);
    double extracted = cJSON_GetNumberValue(first);
    char *printed = cJSON_PrintUnformatted(root);
    int buf_len = 128;
    char *buf = (char *)cJSON_malloc((size_t)buf_len);
    memset(buf, 0, (size_t)buf_len);
    buf[0] = printed[0];
    buf[1] = (char)('0' + ((int)extracted % 10));
    buf[2] = (char)('0' + (((int)extracted / 10) % 10));

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buf);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}