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
//<ID> 891
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
    cJSON *retrieved_array = NULL;
    cJSON *first_item = NULL;
    char *printed = NULL;
    double first_val = 0.0;
    double scalar_val = 0.0;
    int derived_int = 0;
    cJSON_bool replaced = 0;
    cJSON_bool is_num_flag = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", arr);
    num0 = cJSON_CreateNumber(10.5);
    cJSON_AddItemToArray(arr, num0);
    num1 = cJSON_CreateNumber(20.25);
    cJSON_AddItemToArray(arr, num1);
    cJSON_AddNumberToObject(root, "scalar", 3.14);

    // step 3: Operate / Validate
    num_new = cJSON_CreateNumber(99.9);
    replaced = cJSON_ReplaceItemInArray(arr, 0, num_new);
    retrieved_array = cJSON_GetObjectItem(root, "items");
    first_item = cJSON_GetArrayItem(arr, 0);
    is_num_flag = cJSON_IsNumber(first_item);
    first_val = cJSON_GetNumberValue(first_item) * (double)is_num_flag;
    scalar_val = cJSON_GetNumberValue(cJSON_GetObjectItem(root, "scalar"));
    derived_int = (int)(first_val + scalar_val);
    printed = cJSON_Print(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    (void)replaced;
    (void)derived_int;
    return 66;
}