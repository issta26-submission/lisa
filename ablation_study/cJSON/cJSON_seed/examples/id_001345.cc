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
//<ID> 1345
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

    // step 2: Configure
    cJSON_AddItemToObject(root, "items", arr);
    cJSON_AddRawToObject(root, "config", "{\"mode\":\"fast\",\"level\":3}");
    cJSON_AddFalseToObject(root, "enabled");
    cJSON *item_a = cJSON_CreateString("alpha");
    cJSON *item_b = cJSON_CreateNumber(2.0);
    cJSON_AddItemToArray(arr, item_a);
    cJSON_AddItemToArray(arr, item_b);
    cJSON *item_c = cJSON_CreateString("gamma");
    cJSON_AddItemToArray(arr, item_c);

    // step 3: Operate & Validate
    int count = cJSON_GetArraySize(arr);
    cJSON *count_num = cJSON_CreateNumber((double)count);
    cJSON_AddItemToObject(root, "items_count", count_num);
    int buf_len = 64;
    char *buffer = (char *)cJSON_malloc((size_t)buf_len);
    memset(buffer, 0, (size_t)buf_len);
    cJSON *buf_str = cJSON_CreateString(buffer);
    cJSON_AddItemToObject(root, "buffered_string", buf_str);
    cJSON_free(buffer);

    // step 4: Cleanup
    char *printed = cJSON_PrintUnformatted(root);
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}