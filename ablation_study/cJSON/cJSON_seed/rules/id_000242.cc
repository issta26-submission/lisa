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
//<ID> 242
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *num1 = cJSON_CreateNumber(42.0);
    cJSON *num2 = cJSON_CreateNumber(3.5);
    cJSON *meta = cJSON_CreateObject();
    char *printed = NULL;
    char *buffer = (char *)cJSON_malloc(256);
    memset(buffer, 0, 256);
    int arr_size = 0;
    cJSON *tmp0 = NULL;
    cJSON *tmp1 = NULL;
    cJSON *meta_count_item = NULL;
    double v0 = 0.0;
    double v1d = 0.0;
    double count_val = 0.0;

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "values", arr);
    cJSON_AddItemToArray(arr, num1);
    cJSON_AddItemToArray(arr, num2);
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddNumberToObject(meta, "count", 2.0);
    cJSON_AddNumberToObject(meta, "total", 45.5);

    // step 3: Operate / Validate
    arr_size = cJSON_GetArraySize(arr);
    tmp0 = cJSON_GetArrayItem(arr, 0);
    tmp1 = cJSON_GetArrayItem(arr, 1);
    v0 = cJSON_GetNumberValue(tmp0);
    v1d = cJSON_GetNumberValue(tmp1);
    meta_count_item = cJSON_GetObjectItem(meta, "count");
    count_val = cJSON_GetNumberValue(meta_count_item);
    printed = cJSON_PrintUnformatted(root);
    buffer[0] = printed ? printed[0] : '\0';
    (void)arr_size;
    (void)v0;
    (void)v1d;
    (void)count_val;

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}