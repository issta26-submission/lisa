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
//<ID> 195
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *arr = (cJSON *)0;
    cJSON *n1 = (cJSON *)0;
    cJSON *n2 = (cJSON *)0;
    cJSON *n3 = (cJSON *)0;
    cJSON *first = (cJSON *)0;
    cJSON_bool added1 = (cJSON_bool)0;
    cJSON_bool added2 = (cJSON_bool)0;
    cJSON_bool added3 = (cJSON_bool)0;
    int size_before = 0;
    int size_after = 0;
    char *json_text = (char *)0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", arr);

    // step 3: Configure
    n1 = cJSON_CreateNull();
    n2 = cJSON_CreateNull();
    added1 = cJSON_AddItemToArray(arr, n1);
    added2 = cJSON_AddItemToArray(arr, n2);
    size_before = cJSON_GetArraySize(arr);

    // step 4: Operate
    cJSON_DeleteItemFromArray(arr, 1);
    n3 = cJSON_CreateNull();
    added3 = cJSON_AddItemToArray(arr, n3);
    size_after = cJSON_GetArraySize(arr);

    // step 5: Validate
    first = cJSON_GetArrayItem(arr, 0);
    cJSON_AddNumberToObject(root, "size_before", (double)size_before);
    cJSON_AddNumberToObject(root, "size_after", (double)size_after);
    cJSON_AddItemToObject(root, "first_is_null", cJSON_CreateBool(cJSON_IsNull(first)));
    json_text = cJSON_PrintUnformatted(root);
    cJSON_free(json_text);

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}