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
//<ID> 1236
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

    // step 2: Configure
    cJSON *num1 = cJSON_CreateNumber(3.14);
    cJSON_AddItemToArray(arr, num1);
    cJSON *num2 = cJSON_CreateNumber(42.0);
    cJSON_AddItemReferenceToArray(arr, num2);
    cJSON *obj = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "inner", cJSON_CreateString("value"));
    cJSON_AddItemReferenceToObject(root, "ref_obj", obj);

    // step 3: Operate
    int buf_len = 512;
    char *buf = (char *)cJSON_malloc((size_t)buf_len);
    memset(buf, 0, (size_t)buf_len);
    cJSON_PrintPreallocated(root, buf, buf_len, 0);
    cJSON_Minify(buf);
    cJSON_DeleteItemFromArray(arr, 0);

    // step 4: Validate & Cleanup
    int arr_size = cJSON_GetArraySize(arr);
    double remaining_value = cJSON_GetNumberValue(cJSON_GetArrayItem(arr, 0));
    cJSON_AddNumberToObject(root, "count", (double)arr_size);
    (void)remaining_value;
    cJSON_free(buf);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}