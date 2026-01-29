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
//<ID> 1242
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
    cJSON *n1 = cJSON_CreateNumber(10.0);
    cJSON *n2 = cJSON_CreateNumber(20.0);
    cJSON *n3 = cJSON_CreateNumber(30.0);
    cJSON_AddItemToArray(numbers, n1);
    cJSON_AddItemToArray(numbers, n2);
    cJSON_AddItemToArray(numbers, n3);
    cJSON_AddItemToObject(root, "values", numbers);

    // step 2: Configure
    cJSON *dup_numbers = cJSON_Duplicate(numbers, 1);
    cJSON_bool dup_is_array = cJSON_IsArray(dup_numbers);
    cJSON_AddNumberToObject(root, "dup_is_array", (double)dup_is_array);
    cJSON_AddStringToObject(root, "note", "duplicated_array");

    // step 3: Operate & Validate
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(dup_numbers);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}