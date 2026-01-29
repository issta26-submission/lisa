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
//<ID> 1060
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *values = cJSON_AddArrayToObject(root, "values");
    cJSON *enabled_true = cJSON_AddTrueToObject(root, "enabled");
    cJSON *created_bool = cJSON_CreateBool(0);
    cJSON_AddItemToObject(root, "created_bool", created_bool);

    // step 2: Configure
    cJSON *n1 = cJSON_CreateNumber(10.5);
    cJSON_AddItemToArray(values, n1);
    cJSON *n2 = cJSON_CreateNumber(-3.0);
    cJSON_AddItemToArray(values, n2);
    cJSON *inner = cJSON_CreateObject();
    cJSON_AddItemToArray(values, inner);
    cJSON *inner_arr = cJSON_AddArrayToObject(inner, "nested");
    cJSON *s = cJSON_CreateString("hello");
    cJSON_AddItemToArray(inner_arr, s);

    // step 3: Operate and Validate
    cJSON *got_vals = cJSON_GetObjectItem(root, "values");
    int size = cJSON_GetArraySize(got_vals);
    cJSON *first = cJSON_GetArrayItem(got_vals, 0);
    double first_val = cJSON_GetNumberValue(first);
    cJSON_bool inner_is_obj = cJSON_IsObject(cJSON_GetArrayItem(got_vals, 2));
    cJSON *dup_values = cJSON_Duplicate(values, 1);
    char *flat = cJSON_PrintUnformatted(root);
    void *scratch = cJSON_malloc(64);
    memset(scratch, 0, 64);
    ((char *)scratch)[0] = (char)('0' + (size & 0xF));
    ((char *)scratch)[1] = (char)('0' + (inner_is_obj ? 1 : 0));
    cJSON_free(flat);

    // step 4: Cleanup
    cJSON_Delete(root);
    cJSON_Delete(dup_values);
    cJSON_free(scratch);
    // API sequence test completed successfully
    return 66;
}