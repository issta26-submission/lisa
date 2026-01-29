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
//<ID> 1402
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_AddArrayToObject(root, "items");

    // step 2: Configure
    cJSON *count_item = cJSON_AddNumberToObject(root, "count", 3.0);
    cJSON_AddTrueToObject(root, "success");
    cJSON_AddItemToArray(arr, cJSON_CreateNumber(10.0));
    cJSON_AddItemToArray(arr, cJSON_CreateNumber(20.0));
    cJSON_AddItemToArray(arr, cJSON_CreateNumber(30.0));

    // step 3: Operate and Validate
    cJSON *second = cJSON_GetArrayItem(arr, 1);
    cJSON *dup = cJSON_Duplicate(second, 1);
    cJSON_ReplaceItemInArray(arr, 2, dup);
    double second_val = cJSON_GetNumberValue(second);
    double count_val = cJSON_GetNumberValue(count_item);
    char *out = cJSON_PrintUnformatted(root);
    int buf_len = 64;
    char *buf = (char *)cJSON_malloc((size_t)buf_len);
    memset(buf, 0, (size_t)buf_len);
    buf[0] = out ? out[0] : '\0';
    buf[1] = (char)('0' + ((int)second_val % 10));
    buf[2] = (char)('0' + ((int)count_val % 10));
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_free(buf);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}