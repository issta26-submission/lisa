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
//<ID> 1113
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
    cJSON *false_item = cJSON_CreateFalse();

    // step 2: Configure
    cJSON *s1 = cJSON_CreateString("alpha");
    cJSON *s2 = cJSON_CreateString("beta");
    cJSON_AddItemToArray(arr, s1);
    cJSON_AddItemToArray(arr, s2);
    cJSON_AddItemToObject(root, "items", arr);
    cJSON_AddItemToObject(root, "flag", false_item);
    cJSON *note = cJSON_CreateString("metadata");
    cJSON_AddItemToObject(root, "note", note);

    // step 3: Operate and Validate
    cJSON *first = cJSON_GetArrayItem(arr, 0);
    cJSON_bool is_str = cJSON_IsString(first);
    cJSON_bool is_bool = cJSON_IsBool(false_item);
    double computed = 10.0 + (double)(is_str) * 2.0 + (double)(is_bool) * 3.0;
    cJSON *computed_item = cJSON_CreateNumber(computed);
    cJSON_AddItemToObject(root, "computed", computed_item);
    char *out = cJSON_PrintUnformatted(root);
    void *scratch = cJSON_malloc(64);
    memset(scratch, 0, 64);
    ((char *)scratch)[0] = out[0];
    ((char *)scratch)[1] = out[1];
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}