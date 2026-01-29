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
//<ID> 802
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *arr = NULL;
    cJSON *num1 = NULL;
    cJSON *num2 = NULL;
    cJSON *raw = NULL;
    cJSON *size_num = NULL;
    cJSON *item = NULL;
    cJSON *num_cmp = NULL;
    char *printed = NULL;
    int arr_size = 0;
    cJSON_bool cmp_result = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    num1 = cJSON_CreateNumber(10.0);
    cJSON_AddItemToArray(arr, num1);
    num2 = cJSON_CreateNumber(20.0);
    cJSON_AddItemToArray(arr, num2);
    cJSON_AddItemToObject(root, "data", arr);
    raw = cJSON_CreateRaw("{\"meta\":true}");
    cJSON_AddItemToObject(root, "meta_raw", raw);

    // step 3: Operate / Validate
    arr_size = cJSON_GetArraySize(arr);
    size_num = cJSON_CreateNumber((double)arr_size);
    cJSON_AddItemToObject(root, "size", size_num);
    item = cJSON_GetArrayItem(arr, 1);
    num_cmp = cJSON_CreateNumber(20.0);
    cmp_result = cJSON_Compare(item, num_cmp, 1);
    cJSON_AddItemToObject(root, "match", cJSON_CreateBool(cmp_result));
    printed = cJSON_Print(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(num_cmp);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}