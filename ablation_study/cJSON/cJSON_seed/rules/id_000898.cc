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
//<ID> 898
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
    cJSON *num_new = NULL;
    cJSON *retrieved = NULL;
    char *printed = NULL;
    double v0 = 0.0;
    double v1 = 0.0;
    double scalar = 0.0;
    cJSON_bool isnum0 = 0;
    cJSON_bool isnum1 = 0;
    cJSON_bool replaced = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "numbers", arr);
    num0 = cJSON_CreateNumber(10.5);
    cJSON_AddItemToArray(arr, num0);
    num1 = cJSON_CreateNumber(20.25);
    cJSON_AddItemToArray(arr, num1);
    cJSON_AddNumberToObject(root, "scalar", 3.0);

    // step 3: Operate / Validate
    retrieved = cJSON_GetObjectItem(root, "numbers");
    v0 = cJSON_GetNumberValue(cJSON_GetArrayItem(arr, 0));
    v1 = cJSON_GetNumberValue(cJSON_GetArrayItem(arr, 1));
    scalar = cJSON_GetNumberValue(cJSON_GetObjectItem(root, "scalar"));
    isnum0 = cJSON_IsNumber(cJSON_GetArrayItem(arr, 0));
    isnum1 = cJSON_IsNumber(cJSON_GetArrayItem(arr, 1));
    num_new = cJSON_CreateNumber(v0 + v1 + scalar);
    replaced = cJSON_ReplaceItemInArray(arr, 1, num_new);
    printed = cJSON_Print(root);

    (void)retrieved;
    (void)isnum0;
    (void)isnum1;
    (void)replaced;
    (void)v0;
    (void)v1;
    (void)scalar;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}