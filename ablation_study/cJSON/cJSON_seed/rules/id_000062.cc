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
//<ID> 62
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
    cJSON *n1 = cJSON_CreateNumber(10.0);
    cJSON *n2 = cJSON_CreateNumber(20.0);
    cJSON *flag = cJSON_CreateTrue();
    cJSON *found_arr = NULL;
    cJSON *found_flag = NULL;
    cJSON *detached = NULL;
    char *printed = NULL;
    int arr_size = 0;
    double computed = 0.0;
    cJSON_bool is_true = 0;

    // step 2: Setup / Configure structure
    cJSON_AddItemToObject(root, "values", arr);
    cJSON_AddItemReferenceToArray(arr, n1);
    cJSON_AddItemReferenceToArray(arr, n2);
    cJSON_AddItemToObject(root, "ok", flag);

    // step 3: Operate / Validate
    found_arr = cJSON_GetObjectItem(root, "values");
    arr_size = cJSON_GetArraySize(found_arr);
    found_flag = cJSON_GetObjectItem(root, "ok");
    is_true = cJSON_IsTrue(found_flag);
    computed = (double)arr_size + (double)is_true;
    cJSON_AddNumberToObject(root, "computed", computed);
    printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    detached = cJSON_DetachItemFromArray(found_arr, 0);
    cJSON_Delete(detached);
    detached = cJSON_DetachItemFromArray(found_arr, 0);
    cJSON_Delete(detached);
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}