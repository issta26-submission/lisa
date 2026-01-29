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
//<ID> 86
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
    cJSON *n2 = cJSON_CreateNumber(2.0);
    cJSON *t = cJSON_CreateTrue();
    cJSON *first_item = NULL;
    cJSON *third_item = NULL;
    char *printed = NULL;
    int size = 0;
    double first_val = 0.0;
    cJSON_bool is_third_true = 0;
    double total = 0.0;

    // step 2: Setup / Configure structure
    cJSON_AddItemToObject(root, "values", arr);
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToArray(arr, n2);
    cJSON_AddItemToArray(arr, t);
    cJSON_AddNumberToObject(root, "count", (double)cJSON_GetArraySize(arr));

    // step 3: Operate / Validate
    size = cJSON_GetArraySize(arr);
    first_item = cJSON_GetArrayItem(arr, 0);
    third_item = cJSON_GetArrayItem(arr, 2);
    first_val = cJSON_GetNumberValue(first_item);
    is_third_true = cJSON_IsTrue(third_item);
    total = first_val + (double)size + (double)is_third_true;
    cJSON_AddNumberToObject(root, "total", total);
    printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}