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
//<ID> 1407
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
    cJSON_AddItemToObject(root, "numbers", arr);
    cJSON_AddItemToArray(arr, cJSON_CreateNumber(1.0));
    cJSON_AddItemToArray(arr, cJSON_CreateNumber(2.0));
    cJSON_AddItemToArray(arr, cJSON_CreateNumber(3.0));

    // step 2: Configure
    cJSON *scalar = cJSON_AddNumberToObject(root, "scalar", 42.0);
    cJSON *flag = cJSON_AddTrueToObject(root, "flag");
    cJSON *arr_dup = cJSON_Duplicate(arr, 1);
    cJSON_AddItemToObject(root, "numbers_copy", arr_dup);

    // step 3: Operate and Validate
    cJSON_ReplaceItemInArray(arr, 1, cJSON_CreateNumber(99.0));
    int arr_size = cJSON_GetArraySize(arr);
    cJSON *replaced = cJSON_GetArrayItem(arr, 1);
    double replaced_val = cJSON_GetNumberValue(replaced);
    int buf_len = 256;
    char *buf = (char *)cJSON_malloc((size_t)buf_len);
    memset(buf, 0, (size_t)buf_len);
    char *printed = cJSON_Print(root);
    char *scratch = (char *)cJSON_malloc(64);
    memset(scratch, 0, 64);
    buf[0] = printed[0];
    buf[1] = (char)('0' + (arr_size % 10));
    buf[2] = (char)('0' + (((int)replaced_val) % 10));
    scratch[0] = buf[0];
    scratch[1] = buf[1];
    scratch[2] = buf[2];

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buf);
    cJSON_free(scratch);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}