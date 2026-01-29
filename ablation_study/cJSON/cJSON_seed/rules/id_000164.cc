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
//<ID> 164
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *n1 = cJSON_CreateNumber(1.0);
    cJSON *n2 = cJSON_CreateNumber(2.5);
    cJSON *n3 = cJSON_CreateNumber(3.25);

    // step 2: Setup / Configure
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToArray(arr, n2);
    cJSON_AddItemToArray(arr, n3);
    cJSON_AddItemToObject(root, "values", arr);
    cJSON *label_item = cJSON_AddStringToObject(root, "label", "sequence");

    // step 3: Operate / Validate
    int size = cJSON_GetArraySize(arr);
    cJSON *count_item = cJSON_CreateNumber((double)size);
    cJSON_AddItemToObject(root, "count", count_item);
    cJSON *first = cJSON_GetArrayItem(arr, 0);
    double first_value = cJSON_GetNumberValue(first);
    cJSON *first_copy = cJSON_CreateNumber(first_value);
    cJSON_AddItemToObject(root, "first_copy", first_copy);
    char *printed = cJSON_PrintUnformatted(root);
    cJSON_free(printed);
    (void)label_item;

    // step 4: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}