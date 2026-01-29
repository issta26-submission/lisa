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
//<ID> 1553
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *numbers = cJSON_CreateArray();
    cJSON *n1 = cJSON_CreateNumber(1.5);
    cJSON *n2 = cJSON_CreateNumber(2.5);
    cJSON *n3 = cJSON_CreateNumber(3.5);
    cJSON_AddItemToArray(numbers, n1);
    cJSON_AddItemToArray(numbers, n2);
    cJSON_AddItemToArray(numbers, n3);
    cJSON_AddItemToObject(root, "numbers", numbers);

    // step 2: Configure
    char *printed = cJSON_Print(root);
    cJSON *dup_numbers = cJSON_Duplicate(numbers, 1);
    cJSON_AddItemToObject(root, "numbers_copy", dup_numbers);
    char *printed_dup = cJSON_Print(dup_numbers);

    // step 3: Operate & Validate
    int orig_size = cJSON_GetArraySize(numbers);
    int dup_size = cJSON_GetArraySize(dup_numbers);
    cJSON *first_orig = cJSON_GetArrayItem(numbers, 0);
    cJSON *first_dup = cJSON_GetArrayItem(dup_numbers, 0);
    double val_orig = cJSON_GetNumberValue(first_orig);
    double new_val = cJSON_SetNumberHelper(first_dup, val_orig * 2.0);
    (void)orig_size; (void)dup_size; (void)val_orig; (void)new_val;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(printed_dup);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}