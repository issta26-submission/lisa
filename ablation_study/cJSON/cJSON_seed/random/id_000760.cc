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
//<ID> 760
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *child = (cJSON *)0;
    cJSON *arr = (cJSON *)0;
    cJSON *str0 = (cJSON *)0;
    cJSON *str1 = (cJSON *)0;
    cJSON *retrieved = (cJSON *)0;
    char *retrieved_value = (char *)0;
    char *printed = (char *)0;
    cJSON_bool is_root_object = 0;
    int validation_score = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    child = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    str0 = cJSON_CreateString("alpha");
    str1 = cJSON_CreateString("beta");

    // step 3: Configure
    cJSON_AddItemToObject(root, "metadata", child);
    cJSON_AddItemToObject(child, "names", arr);
    cJSON_AddItemToArray(arr, str0);
    cJSON_AddItemToArray(arr, str1);

    // step 4: Operate
    retrieved = cJSON_GetArrayItem(arr, 0);
    retrieved_value = cJSON_GetStringValue(retrieved);
    is_root_object = cJSON_IsObject(root);
    printed = cJSON_PrintUnformatted(root);

    // step 5: Validate
    validation_score = (retrieved_value != (char *)0) + (int)is_root_object + (printed != (char *)0);
    (void)validation_score;

    // step 6: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}