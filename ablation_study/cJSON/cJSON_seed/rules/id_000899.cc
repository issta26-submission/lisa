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
//<ID> 899
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
    cJSON *num_replacement = NULL;
    cJSON *retrieved_arr = NULL;
    char *printed = NULL;
    cJSON_bool is_num0 = 0;
    cJSON_bool is_num1 = 0;
    cJSON_bool replaced = 0;
    double val0 = 0.0;
    double val1 = 0.0;
    int size_before = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "arr", arr);
    num0 = cJSON_CreateNumber(1.5);
    cJSON_AddItemToArray(arr, num0);
    num1 = cJSON_CreateNumber(2.5);
    cJSON_AddItemToArray(arr, num1);
    num_replacement = cJSON_CreateNumber(9.25);

    // step 3: Operate / Validate
    retrieved_arr = cJSON_GetObjectItem(root, "arr");
    size_before = cJSON_GetArraySize(retrieved_arr);
    is_num0 = cJSON_IsNumber(cJSON_GetArrayItem(retrieved_arr, 0));
    is_num1 = cJSON_IsNumber(cJSON_GetArrayItem(retrieved_arr, 1));
    replaced = cJSON_ReplaceItemInArray(retrieved_arr, 1, num_replacement);
    val0 = cJSON_GetNumberValue(cJSON_GetArrayItem(retrieved_arr, 0));
    val1 = cJSON_GetNumberValue(cJSON_GetArrayItem(retrieved_arr, 1));
    printed = cJSON_Print(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    (void)is_num0;
    (void)is_num1;
    (void)replaced;
    (void)val0;
    (void)val1;
    (void)size_before;
    return 66;
}