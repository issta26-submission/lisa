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
//<ID> 1128
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *arr = (cJSON *)0;
    cJSON *num_in_root = (cJSON *)0;
    cJSON *num_in_array = (cJSON *)0;
    cJSON *retrieved = (cJSON *)0;
    char *printed = (char *)0;
    double retrieved_value = 0.0;
    cJSON_bool is_number = (cJSON_bool)0;

    // step 2: Setup - create root object and an array
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();

    // step 3: Configure - add a number to root and a number into the array, then attach the array using CS variant
    num_in_root = cJSON_AddNumberToObject(root, "count", 123.0);
    num_in_array = cJSON_CreateNumber(7.0);
    cJSON_AddItemToArray(arr, num_in_array);
    cJSON_AddItemToObjectCS(root, "values", arr);

    // step 4: Operate - retrieve and inspect the number, serialize the root
    retrieved = cJSON_GetObjectItem(root, "count");
    retrieved_value = cJSON_GetNumberValue(retrieved);
    is_number = cJSON_IsNumber(retrieved);
    printed = cJSON_PrintUnformatted(root);
    cJSON_free((void *)printed);
    (void)retrieved_value;
    (void)is_number;
    (void)num_in_root;
    (void)num_in_array;

    // step 5: Cleanup - delete root which frees attached items
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}