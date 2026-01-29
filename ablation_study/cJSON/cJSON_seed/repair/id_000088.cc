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
//<ID> 88
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
    cJSON_AddItemToObject(root, "numbers", numbers);
    cJSON *n1 = cJSON_CreateNumber(1.0);
    cJSON_AddItemToArray(numbers, n1);
    cJSON *n2 = cJSON_CreateNumber(2.5);
    cJSON_AddItemToArray(numbers, n2);
    cJSON *n3 = cJSON_CreateNumber(3.0);
    cJSON_AddItemToArray(numbers, n3);

    // step 2: Configure and Operate
    cJSON *dup_numbers = cJSON_Duplicate(numbers, 1);
    cJSON_AddItemToObject(root, "numbers_backup", dup_numbers);
    cJSON *n4 = cJSON_CreateNumber(4.5);
    cJSON_AddItemToArray(dup_numbers, n4);

    // step 3: Validate
    int orig_size = cJSON_GetArraySize(numbers);
    int dup_size = cJSON_GetArraySize(dup_numbers);
    cJSON_AddNumberToObject(root, "orig_size", (double)orig_size);
    cJSON_AddNumberToObject(root, "dup_size", (double)dup_size);
    cJSON *first_item = cJSON_GetArrayItem(numbers, 0);
    double first_val = cJSON_GetNumberValue(first_item);
    cJSON_AddNumberToObject(root, "first_value", first_val);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}