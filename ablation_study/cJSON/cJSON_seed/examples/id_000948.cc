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
//<ID> 948
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
    cJSON *n1 = cJSON_CreateNumber(3.14);
    cJSON *n2 = cJSON_CreateNumber(2.718);
    cJSON *btrue = cJSON_CreateTrue();
    cJSON *bfalse = cJSON_CreateFalse();

    // step 2: Configure
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToArray(arr, btrue);
    cJSON_InsertItemInArray(arr, 1, n2);
    cJSON_AddItemToArray(arr, bfalse);
    cJSON_AddItemToObject(root, "values", arr);
    cJSON *num_copy = cJSON_Duplicate(n1, 1);
    cJSON_AddItemToObject(root, "first_copy", num_copy);

    // step 3: Operate & Validate
    cJSON *got_n1 = cJSON_GetArrayItem(arr, 0);
    double val0 = cJSON_GetNumberValue(got_n1);
    cJSON *got_n2 = cJSON_GetArrayItem(arr, 1);
    double val1 = cJSON_GetNumberValue(got_n2);
    cJSON *got_btrue = cJSON_GetArrayItem(arr, 2);
    cJSON_bool is_bool = cJSON_IsBool(got_btrue);
    cJSON_bool cmp = cJSON_Compare(got_n1, num_copy, 1);
    cJSON *result_bool = cJSON_CreateBool((cJSON_bool)(is_bool && cmp));
    cJSON_AddItemToObject(root, "bool_check", result_bool);
    (void)val0;
    (void)val1;

    // step 4: Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}