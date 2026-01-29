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
//<ID> 80
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
    cJSON *n1 = cJSON_CreateNumber(3.14);
    cJSON *n2 = cJSON_CreateNumber(2.0);
    cJSON *t = cJSON_CreateTrue();
    cJSON *first_item = NULL;
    cJSON *true_item = NULL;
    char *printed = NULL;
    double first_val = 0.0;
    double count_val = 0.0;
    double true_val = 0.0;
    double total = 0.0;
    cJSON_bool is_true_flag = 0;

    // step 2: Setup / Configure structure
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToArray(arr, n2);
    cJSON_AddItemToArray(arr, t);
    cJSON_AddItemToObject(root, "values", arr);
    cJSON_AddNumberToObject(root, "count", (double)cJSON_GetArraySize(arr));

    // step 3: Operate / Validate
    first_item = cJSON_GetArrayItem(arr, 0);
    first_val = cJSON_GetNumberValue(first_item);
    count_val = (double)cJSON_GetArraySize(arr);
    true_item = cJSON_GetArrayItem(arr, 2);
    is_true_flag = cJSON_IsTrue(true_item);
    true_val = (double)is_true_flag;
    total = first_val + count_val + true_val;
    cJSON_AddNumberToObject(root, "total", total);
    printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}