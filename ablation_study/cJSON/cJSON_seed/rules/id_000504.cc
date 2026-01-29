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
//<ID> 504
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    char *buffer = (char *)cJSON_malloc(64);
    memset(buffer, 0, 64);
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *arr2 = cJSON_CreateArray();
    cJSON *n0 = cJSON_CreateNumber(1.5);
    cJSON *n1 = cJSON_CreateNumber(2.5);
    cJSON *m0 = cJSON_CreateNumber(1.5);
    cJSON *m1 = cJSON_CreateNumber(2.5);
    char *printed = NULL;
    cJSON_bool cmp = 0;
    cJSON *retr_arr = NULL;
    cJSON *retr_item = NULL;
    double retrieved_value = 0.0;

    // step 2: Setup / Configure
    cJSON_AddItemToArray(arr, n0);
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToArray(arr2, m0);
    cJSON_AddItemToArray(arr2, m1);
    cJSON_AddItemToObject(root, "numbers", arr);
    cJSON_AddItemToObject(root, "numbers_copy", arr2);

    // step 3: Operate / Validate
    printed = cJSON_Print(root);
    cmp = cJSON_Compare(arr, arr2, 1);
    retr_arr = cJSON_GetObjectItem(root, "numbers");
    retr_item = cJSON_GetArrayItem(retr_arr, 1);
    retrieved_value = cJSON_GetNumberValue(retr_item);
    buffer[0] = (char)('0' + (char)cmp);
    buffer[1] = (char)('0' + (int)retrieved_value);
    buffer[2] = '\0';

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}