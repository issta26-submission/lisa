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
//<ID> 1627
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "values", arr);
    cJSON *s1 = cJSON_CreateString("alpha");
    cJSON *s2 = cJSON_CreateString("beta");
    cJSON *s3 = cJSON_CreateString("gamma");
    cJSON_AddItemToArray(arr, s1);
    cJSON_AddItemToArray(arr, s2);
    cJSON_AddItemToArray(arr, s3);

    // step 2: Configure
    cJSON *nil = cJSON_CreateNull();
    cJSON_AddItemToObject(root, "nil", nil);
    cJSON_bool nil_flag = cJSON_IsNull(nil);
    cJSON_AddNumberToObject(root, "nil_is_null", (double)nil_flag);

    // step 3: Operate & Validate
    cJSON *second = cJSON_GetArrayItem(arr, 1);
    char *second_str = cJSON_GetStringValue(second);
    cJSON *second_copy = cJSON_CreateString(second_str);
    cJSON_AddItemToObject(root, "second_copy", second_copy);
    cJSON_AddItemReferenceToArray(arr, second);
    char *unformatted = cJSON_PrintUnformatted(root);
    cJSON_free(unformatted);

    // step 4: Cleanup
    char *full = cJSON_Print(root);
    cJSON_free(full);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}