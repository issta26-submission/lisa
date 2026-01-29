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
//<ID> 1189
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
    cJSON *num = cJSON_CreateNumber(123.0);
    cJSON_AddItemToArray(arr, num);
    cJSON *str = cJSON_CreateString("original");
    cJSON_AddItemToArray(arr, str);

    // step 2: Configure
    cJSON_AddItemReferenceToArray(arr, num);
    cJSON_bool arr_flag = cJSON_IsArray(arr);
    cJSON *replacement = cJSON_CreateNumber(999.0);
    cJSON_ReplaceItemViaPointer(arr, str, replacement);

    // step 3: Operate
    cJSON_AddNullToObject(root, "nil");
    char *flat = cJSON_PrintUnformatted(root);
    int buf_len = 128;
    char *buf = (char *)cJSON_malloc((size_t)buf_len);
    memset(buf, 0, (size_t)buf_len);

    // step 4: Validate & Cleanup
    (void)arr_flag;
    cJSON_free(flat);
    cJSON_free(buf);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}