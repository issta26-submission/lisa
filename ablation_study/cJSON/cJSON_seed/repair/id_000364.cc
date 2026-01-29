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
//<ID> 364
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *objA = cJSON_AddObjectToObject(root, "objA");
    cJSON *values = cJSON_CreateArray();
    cJSON *n1 = cJSON_CreateNumber(3.0);
    cJSON *n2 = cJSON_CreateNumber(7.0);
    cJSON_AddItemToArray(values, n1);
    cJSON_AddItemToArray(values, n2);
    cJSON_AddItemToObject(root, "values", values);
    cJSON_AddNullToObject(objA, "optional");

    // step 2: Configure
    cJSON *meta = cJSON_AddObjectToObject(objA, "meta");
    cJSON_AddNumberToObject(meta, "mult", 2.0);
    cJSON_AddStringToObject(meta, "note", "calculation");

    // step 3: Operate and Validate
    cJSON *got_values = cJSON_GetObjectItemCaseSensitive(root, "values");
    cJSON_bool is_arr = cJSON_IsArray(got_values);
    int count = cJSON_GetArraySize(got_values);
    cJSON *first = cJSON_GetArrayItem(got_values, 0);
    double first_val = cJSON_GetNumberValue(first);
    double mult_val = cJSON_GetNumberValue(cJSON_GetObjectItemCaseSensitive(meta, "mult"));
    double computed = first_val * mult_val * (double)is_arr + (double)count;
    cJSON *result = cJSON_CreateNumber(computed);
    cJSON_AddItemToObject(root, "computed", result);

    // step 4: Cleanup
    char *out = cJSON_Print(root);
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}