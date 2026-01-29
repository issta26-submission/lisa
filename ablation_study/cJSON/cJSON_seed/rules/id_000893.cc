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
//<ID> 893
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
    cJSON *newnum = NULL;
    cJSON *retrieved_scalar = NULL;
    cJSON *retrieved_items = NULL;
    char *printed = NULL;
    cJSON_bool is_number_item = 0;
    double scalar_val = 0.0;
    int derived_int = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", arr);
    num0 = cJSON_CreateNumber(10.5);
    num1 = cJSON_CreateNumber(20.75);
    cJSON_AddItemToArray(arr, num0);
    cJSON_AddItemToArray(arr, num1);
    cJSON_AddNumberToObject(root, "scalar", 3.0);

    // step 3: Operate / Validate
    retrieved_scalar = cJSON_GetObjectItem(root, "scalar");
    scalar_val = cJSON_GetNumberValue(retrieved_scalar);
    retrieved_items = cJSON_GetObjectItem(root, "items");
    is_number_item = cJSON_IsNumber(cJSON_GetArrayItem(retrieved_items, 1));
    newnum = cJSON_CreateNumber(99.9);
    cJSON_ReplaceItemInArray(retrieved_items, 0, newnum);
    derived_int = (int)(scalar_val + cJSON_GetNumberValue(cJSON_GetArrayItem(retrieved_items, 1)));
    printed = cJSON_Print(root);

    // step 4: Cleanup
    cJSON_free(printed);
    (void)is_number_item;
    (void)derived_int;
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}