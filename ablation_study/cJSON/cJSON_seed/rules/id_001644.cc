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
//<ID> 1644
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
    cJSON *found_arr = NULL;
    char *out = NULL;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "numbers", arr);
    num1 = cJSON_CreateNumber(42.0);
    cJSON_AddItemToArray(arr, num1);
    num2 = cJSON_CreateNumber(3.14159);
    cJSON_AddItemToArray(arr, num2);

    // create a reference entry to the same array without transferring ownership
    cJSON_AddItemReferenceToObject(root, "numbers_ref", arr);
    arr_ref = cJSON_GetObjectItem(root, "numbers_ref");
    found_arr = cJSON_GetObjectItem(root, "numbers");
    out = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}