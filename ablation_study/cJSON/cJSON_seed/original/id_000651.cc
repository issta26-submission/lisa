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
//<ID> 651
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize & Configure
    cJSON *root = cJSON_CreateObject();
    cJSON *numbers = cJSON_AddArrayToObject(root, "numbers");
    cJSON *n1 = cJSON_CreateNumber(1.23);
    cJSON *n2 = cJSON_CreateNumber(4.56);
    cJSON_AddItemToArray(numbers, n1);
    cJSON_AddItemToArray(numbers, n2);
    cJSON *sref = cJSON_CreateStringReference("hello");
    cJSON_AddItemToObject(root, "greeting", sref);

    // step 2: Operate
    cJSON *second = cJSON_GetArrayItem(numbers, 1);
    cJSON_bool second_is_number = cJSON_IsNumber(second);
    cJSON_AddBoolToObject(root, "second_is_number", second_is_number);
    cJSON *detached = cJSON_DetachItemViaPointer(numbers, second);
    cJSON_AddItemToObject(root, "detached_second", detached);

    // step 3: Validate
    cJSON *first = cJSON_GetArrayItem(numbers, 0);
    cJSON_bool first_invalid = cJSON_IsInvalid(first);
    cJSON_AddBoolToObject(root, "first_is_invalid", first_invalid);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    char buffer[256];
    memset(buffer, 0, sizeof(buffer));
    cJSON_PrintPreallocated(root, buffer, (int)sizeof(buffer), 0);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}