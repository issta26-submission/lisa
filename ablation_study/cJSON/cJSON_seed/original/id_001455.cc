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
//<ID> 1455
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
    cJSON *n1 = cJSON_CreateNumber(10.0);
    cJSON *n2 = cJSON_CreateNumber(20.5);
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToArray(arr, n2);

    // step 2: Configure
    cJSON *arr_ref = cJSON_CreateArrayReference(arr);
    cJSON_AddItemToObject(root, "numbers", arr_ref);
    cJSON *n3 = cJSON_CreateNumber(42.25);
    cJSON_AddItemToArray(arr, n3);

    // step 3: Operate & Validate
    int buf_len = 512;
    char *buffer = (char *)cJSON_malloc((size_t)buf_len);
    memset(buffer, 0, (size_t)buf_len);
    cJSON_bool printed = cJSON_PrintPreallocated(root, buffer, buf_len, 0);
    cJSON_bool has_numbers = cJSON_HasObjectItem(root, "numbers");
    int size_now = cJSON_GetArraySize(arr);
    (void)printed;
    (void)has_numbers;
    (void)size_now;

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(root);
    cJSON_Delete(arr);
    // API sequence test completed successfully
    return 66;
}