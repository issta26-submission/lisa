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
//<ID> 66
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
    cJSON *arr_ref = cJSON_CreateArray();
    cJSON *n1 = cJSON_CreateNumber(10.0);
    cJSON *n2 = cJSON_CreateNumber(20.0);
    cJSON *btrue = cJSON_CreateTrue();
    cJSON *retrieved = NULL;
    cJSON *first_elem = NULL;
    cJSON *status_item = NULL;
    cJSON *computed_item = NULL;
    char *printed = NULL;
    double first_val = 0.0;
    double computed = 0.0;
    int arr_size = 0;
    cJSON_bool ref_added = 0;
    cJSON_bool is_true_val = 0;

    // step 2: Setup / Configure structure
    cJSON_AddItemToObject(root, "numbers", arr);
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToArray(arr, n2);
    cJSON_AddItemToObject(root, "status", btrue);
    ref_added = cJSON_AddItemReferenceToArray(arr_ref, n1);
    cJSON_AddItemToObject(root, "refarr", arr_ref);

    // step 3: Operate / Validate
    retrieved = cJSON_GetObjectItem(root, "numbers");
    first_elem = cJSON_GetArrayItem(retrieved, 0);
    first_val = cJSON_GetNumberValue(first_elem);
    status_item = cJSON_GetObjectItem(root, "status");
    is_true_val = cJSON_IsTrue(status_item);
    arr_size = cJSON_GetArraySize(arr);
    computed = first_val + (double)arr_size + (double)is_true_val * 100.0;
    computed_item = cJSON_AddNumberToObject(root, "computed", computed);
    printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}