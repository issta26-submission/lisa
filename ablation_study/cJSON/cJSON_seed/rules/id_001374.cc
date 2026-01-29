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
//<ID> 1374
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
    cJSON *num0 = NULL;
    cJSON *num1 = NULL;
    cJSON *num_copy = NULL;
    cJSON *flagA = NULL;
    cJSON *flagB = NULL;
    cJSON *retrieved = NULL;
    char *printed = NULL;
    int arr_size = 0;
    cJSON_bool same_num = 0;
    cJSON_bool same_flag = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    cJSON_AddNumberToObject(root, "threshold", 42.0);
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "values", arr);
    num0 = cJSON_CreateNumber(3.14);
    num1 = cJSON_CreateNumber(2.718);
    cJSON_AddItemToArray(arr, num0);
    cJSON_AddItemToArray(arr, num1);
    flagA = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "enabled", flagA);
    num_copy = cJSON_CreateNumber(3.14);
    flagB = cJSON_CreateTrue();

    // step 3: Operate / Validate
    arr_size = cJSON_GetArraySize(arr);
    retrieved = cJSON_GetArrayItem(arr, 0);
    same_num = cJSON_Compare(retrieved, num_copy, 1);
    same_flag = cJSON_Compare(flagA, flagB, 1);
    printed = cJSON_PrintUnformatted(root);
    (void)arr_size;
    (void)same_num;
    (void)same_flag;
    (void)retrieved;
    (void)printed;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(num_copy);
    cJSON_Delete(flagB);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}