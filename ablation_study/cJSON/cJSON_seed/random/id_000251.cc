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
//<ID> 251
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    const int numbers[] = {10, 20, 30, 40};
    cJSON *int_array = (cJSON *)0;
    cJSON *label_item = (cJSON *)0;
    cJSON *count_item = (cJSON *)0;
    cJSON_bool added_array = (cJSON_bool)0;
    int array_size = 0;
    char *printed = (char *)0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    int_array = cJSON_CreateIntArray(numbers, 4);

    // step 3: Configure
    added_array = cJSON_AddItemToObject(root, "ints", int_array);
    label_item = cJSON_AddStringToObject(root, "label", "integer array");

    // step 4: Operate
    array_size = cJSON_GetArraySize(int_array);
    count_item = cJSON_CreateNumber((double)array_size);
    cJSON_AddItemToObject(root, "count", count_item);

    // step 5: Validate
    printed = cJSON_PrintUnformatted(root);
    cJSON_free(printed);

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}