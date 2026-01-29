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
//<ID> 1230
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

    // step 2: Configure
    cJSON *num1 = cJSON_CreateNumber(3.14);
    cJSON_AddItemToArray(arr, num1);
    cJSON *shared_num = cJSON_CreateNumber(99.0);
    cJSON_AddItemReferenceToArray(arr, shared_num);
    cJSON_AddItemReferenceToObject(root, "shared", shared_num);
    cJSON *num2 = cJSON_CreateNumber(42.0);
    cJSON_AddItemToArray(arr, num2);

    // step 3: Operate
    char *printed = cJSON_PrintUnformatted(root);
    int printed_len = (int)strlen(printed);
    char *buf = (char *)cJSON_malloc((size_t)printed_len + 16);
    memset(buf, 0, (size_t)printed_len + 16);
    memcpy(buf, printed, (size_t)printed_len + 1);
    cJSON_Minify(buf);
    int count_before = cJSON_GetArraySize(arr);
    cJSON_DeleteItemFromArray(arr, 1);
    int count_after = cJSON_GetArraySize(arr);
    double first_value = cJSON_GetNumberValue(cJSON_GetArrayItem(arr, 0));
    (void)count_before;
    (void)count_after;
    (void)first_value;

    // step 4: Validate & Cleanup
    cJSON_free(printed);
    cJSON_free(buf);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}