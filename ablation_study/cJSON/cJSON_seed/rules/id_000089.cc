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
//<ID> 89
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
    cJSON *n1 = cJSON_CreateNumber(42.0);
    cJSON *n2 = cJSON_CreateNumber(3.5);
    cJSON *t = cJSON_CreateTrue();
    cJSON *true_item = NULL;
    char *printed = NULL;
    int size = 0;
    double first_value = 0.0;
    double total = 0.0;
    cJSON_bool is_true = 0;

    // step 2: Setup / Configure structure
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToArray(arr, n2);
    cJSON_AddItemToArray(arr, t);
    cJSON_AddItemToObject(root, "items", arr);

    // step 3: Operate / Validate
    size = cJSON_GetArraySize(arr);
    cJSON_AddNumberToObject(root, "count", (double)size);
    first_value = cJSON_GetNumberValue(cJSON_GetArrayItem(arr, 0));
    true_item = cJSON_GetArrayItem(arr, 2);
    is_true = cJSON_IsTrue(true_item);
    total = (double)size + first_value + (double)is_true;
    cJSON_AddNumberToObject(root, "total", total);
    printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}