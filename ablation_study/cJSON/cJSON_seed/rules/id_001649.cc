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
//<ID> 1649
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *arr = NULL;
    cJSON *num1 = NULL;
    cJSON *num2 = NULL;
    cJSON *arr_ref = NULL;
    cJSON *found = NULL;
    char *out = NULL;
    int arr_size = 0;
    char buffer[256];
    memset(buffer, 0, sizeof(buffer));

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "numbers", arr);
    num1 = cJSON_CreateNumber(3.14);
    num2 = cJSON_CreateNumber(42.0);

    // step 3: Operate / Validate
    cJSON_AddItemToArray(arr, num1);
    cJSON_AddItemToArray(arr, num2);
    /* Use a safe duplicate instead of CreateArrayReference to avoid
       double-free issues when deleting the root that contains both
       the original array and the referenced item. */
    arr_ref = cJSON_Duplicate(arr, 1);
    cJSON_AddItemToObject(root, "numbers_ref", arr_ref);
    found = cJSON_GetObjectItem(root, "numbers");
    arr_size = cJSON_GetArraySize(found);
    out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);
    return 66;
}