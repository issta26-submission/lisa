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
//<ID> 1108
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *inner = cJSON_AddObjectToObject(root, "inner");
    cJSON *arr = cJSON_CreateArray();
    cJSON *n1 = cJSON_CreateNumber(1.0);
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToObject(inner, "list", arr);
    cJSON *flag = cJSON_AddFalseToObject(inner, "flag");

    // step 2: Configure
    cJSON *arr_ref = cJSON_CreateArrayReference(arr);
    cJSON_AddItemToObject(root, "list_ref", arr_ref);
    cJSON *note = cJSON_CreateString("metadata");
    cJSON_AddItemToObject(root, "note", note);

    // step 3: Operate and Validate
    cJSON_bool is_bool = cJSON_IsBool(flag);
    cJSON_bool is_false = cJSON_IsFalse(flag);
    double computed = 10.0 + (double)(is_bool) * 2.0 + (double)(is_false) * 3.0;
    cJSON *computed_item = cJSON_CreateNumber(computed);
    cJSON_AddItemToObject(root, "computed", computed_item);
    char *out = cJSON_PrintUnformatted(root);
    void *scratch = cJSON_malloc(32);
    memset(scratch, 0, 32);
    ((char *)scratch)[0] = out[0];
    ((char *)scratch)[1] = out[1];
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}