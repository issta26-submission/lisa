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
//<ID> 1233
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
    cJSON *num1 = cJSON_CreateNumber(3.1415);
    cJSON_AddItemToArray(arr, num1);
    cJSON *num2 = cJSON_CreateNumber(42.0);
    cJSON_AddItemToObject(root, "primary", num2);
    cJSON_bool ref_added_to_array = cJSON_AddItemReferenceToArray(arr, num2);
    cJSON_bool ref_added_to_object = cJSON_AddItemReferenceToObject(root, "primary_ref", num2);

    // step 3: Operate
    char *printed = cJSON_PrintUnformatted(root);
    size_t printed_len = printed ? strlen(printed) : 0;
    int buf_len = (int)printed_len + 64;
    char *buf = (char *)cJSON_malloc((size_t)buf_len);
    memset(buf, 0, (size_t)buf_len);
    printed ? memcpy(buf, printed, printed_len + 1) : memcpy(buf, "", 1);
    cJSON_Minify(buf);
    int arr_size_before = cJSON_GetArraySize(arr);
    double first_val = cJSON_GetNumberValue(cJSON_GetArrayItem(arr, 0));
    cJSON_DeleteItemFromArray(arr, 0);
    cJSON_AddNumberToObject(root, "extracted_first", first_val);
    (void)ref_added_to_array;
    (void)ref_added_to_object;
    (void)arr_size_before;

    // step 4: Validate & Cleanup
    cJSON_free(printed);
    cJSON_free(buf);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}