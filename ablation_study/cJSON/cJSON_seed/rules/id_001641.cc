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
//<ID> 1641
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
    cJSON *num3 = NULL;
    cJSON *arr_ref = NULL;
    cJSON *found_ref = NULL;
    char *out = NULL;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "numbers", arr);
    num1 = cJSON_CreateNumber(3.14);
    cJSON_AddItemToArray(arr, num1);
    num2 = cJSON_CreateNumber(2.71);
    cJSON_AddItemToArray(arr, num2);

    // create a reference to the existing array and add it to the object
    cJSON_AddItemReferenceToObject(root, "numbers_ref", arr);

    // step 3: Operate / Validate
    found_ref = cJSON_GetObjectItem(root, "numbers_ref");
    num3 = cJSON_CreateNumber(42.0);
    cJSON_AddItemToArray(found_ref, num3);
    out = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}