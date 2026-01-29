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
//<ID> 54
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
    cJSON_AddItemToObject(root, "numbers", num_array);

    // step 2: Configure
    cJSON *num_a = cJSON_CreateNumber(3.14159);
    cJSON *num_b = cJSON_CreateNumber(2.71828);
    cJSON *num_c = cJSON_CreateNumber(1.61803);
    cJSON_AddItemToArray(num_array, num_a);
    cJSON_AddItemToArray(num_array, num_b);
    cJSON_AddItemToArray(num_array, num_c);

    // step 3: Operate & Validate
    cJSON *item0 = cJSON_GetArrayItem(num_array, 0);
    cJSON *item1 = cJSON_GetArrayItem(num_array, 1);
    cJSON *item2 = cJSON_GetArrayItem(num_array, 2);
    double val0 = cJSON_GetNumberValue(item0);
    double val1 = cJSON_GetNumberValue(item1);
    double val2 = cJSON_GetNumberValue(item2);
    double total = val0 + val1 + val2;
    cJSON *total_item = cJSON_CreateNumber(total);
    cJSON_AddItemToObject(root, "sum", total_item);
    char *printed = cJSON_PrintUnformatted(root);
    cJSON_free(printed);

    // step 4: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}