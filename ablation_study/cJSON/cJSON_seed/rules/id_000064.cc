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
//<ID> 64
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
    cJSON *n2 = cJSON_CreateNumber(7.0);
    cJSON *true_item = cJSON_CreateTrue();
    cJSON *count_item = NULL;
    cJSON *ok_item = NULL;
    cJSON *sum_item = NULL;
    int arr_size = 0;
    double count_val = 0.0;
    cJSON_bool is_ok = 0;
    double sum_val = 0.0;

    // step 2: Setup / Configure structure
    cJSON_AddItemToObject(root, "values", arr);
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemReferenceToArray(arr, n2);
    cJSON_AddItemToObject(root, "ok", true_item);
    cJSON_AddNumberToObject(root, "count", 1.0);

    // step 3: Operate / Validate
    count_item = cJSON_GetObjectItem(root, "count");
    count_val = cJSON_GetNumberValue(count_item);
    ok_item = cJSON_GetObjectItem(root, "ok");
    is_ok = cJSON_IsTrue(ok_item);
    arr_size = cJSON_GetArraySize(arr);
    sum_val = count_val + (double)arr_size + (double)is_ok;
    sum_item = cJSON_AddNumberToObject(root, "sum", sum_val);

    // step 4: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}