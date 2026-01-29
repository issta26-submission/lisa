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
//<ID> 701
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *num_array = cJSON_CreateArray();
    cJSON *n0 = cJSON_CreateNumber(1.0);
    cJSON *n1 = cJSON_CreateNumber(2.0);
    cJSON *n2 = cJSON_CreateNumber(3.0);
    cJSON_AddItemToArray(num_array, n0);
    cJSON_AddItemToArray(num_array, n1);
    cJSON_AddItemToArray(num_array, n2);
    cJSON_AddItemToObject(root, "numbers", num_array);

    // step 2: Configure
    cJSON *new_num = cJSON_CreateNumber(42.0);
    cJSON_ReplaceItemInArray(num_array, 1, new_num);

    // step 3: Operate
    int size = cJSON_GetArraySize(num_array);
    cJSON *replaced = cJSON_GetArrayItem(num_array, 1);
    double replaced_val = cJSON_GetNumberValue(replaced);
    cJSON *replaced_copy = cJSON_CreateNumber(replaced_val);
    cJSON_AddItemToObject(root, "replaced_value", replaced_copy);
    char buffer[256];
    memset(buffer, 0, sizeof(buffer));
    cJSON_PrintPreallocated(root, buffer, (int)sizeof(buffer), 0);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Validate & Cleanup
    cJSON_AddNumberToObject(root, "numbers_count", (double)size);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}