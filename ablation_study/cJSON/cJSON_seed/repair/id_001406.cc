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
//<ID> 1406
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "arr", arr);

    // step 2: Configure
    cJSON_AddNumberToObject(root, "count", 3.0);
    cJSON_AddTrueToObject(root, "valid");
    cJSON *item0 = cJSON_CreateNumber(1.0);
    cJSON *item1 = cJSON_CreateNumber(2.0);
    cJSON *item2 = cJSON_CreateNumber(3.0);
    cJSON_AddItemToArray(arr, item0);
    cJSON_AddItemToArray(arr, item1);
    cJSON_AddItemToArray(arr, item2);

    // step 3: Operate and Validate
    cJSON *second = cJSON_GetArrayItem(arr, 1);
    cJSON *dup = cJSON_Duplicate(second, 1);
    cJSON_bool replaced = cJSON_ReplaceItemInArray(arr, 0, dup);
    cJSON *new_first = cJSON_GetArrayItem(arr, 0);
    double new_first_val = cJSON_GetNumberValue(new_first);
    cJSON *count_item = cJSON_GetObjectItem(root, "count");
    double count_val = cJSON_GetNumberValue(count_item);
    char *printed = cJSON_PrintUnformatted(root);
    int buf_len = 128;
    char *buf = (char *)cJSON_malloc((size_t)buf_len);
    memset(buf, 0, (size_t)buf_len);
    buf[0] = printed ? printed[0] : '\0';
    buf[1] = (char)('0' + ((int)new_first_val % 10));
    buf[2] = (char)('0' + ((int)count_val % 10));
    cJSON_free(printed);

    // step 4: Cleanup
    cJSON_free(buf);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}