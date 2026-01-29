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
//<ID> 1223
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
    cJSON_AddItemToObject(root, "items", arr);
    cJSON *s1 = cJSON_CreateString("hello");
    cJSON_AddItemToArray(arr, s1);
    cJSON *n1 = cJSON_CreateNumber(3.14);
    cJSON_AddItemToArray(arr, n1);

    // step 2: Configure
    cJSON *s2 = cJSON_CreateString("world");
    cJSON_InsertItemInArray(arr, 1, s2);
    cJSON *btrue = cJSON_CreateTrue();
    cJSON_AddItemToArray(arr, btrue);

    // step 3: Operate
    char *printed = cJSON_Print(root);
    int buf_len = 256;
    char *buf = (char *)cJSON_malloc((size_t)buf_len);
    memset(buf, 0, (size_t)buf_len);
    cJSON_PrintPreallocated(root, buf, buf_len, 1);
    cJSON_Minify(buf);
    const char *ver = cJSON_Version();
    cJSON_bool arr_check = cJSON_IsArray(arr);
    cJSON *first_item = cJSON_GetArrayItem(arr, 0);
    char *first_str = cJSON_GetStringValue(first_item);

    // step 4: Validate & Cleanup
    (void)printed;
    (void)ver;
    (void)arr_check;
    (void)first_str;
    cJSON_free(printed);
    cJSON_free(buf);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}