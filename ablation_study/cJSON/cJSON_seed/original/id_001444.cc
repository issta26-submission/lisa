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
//<ID> 1444
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *settings = cJSON_AddObjectToObject(root, "settings");
    cJSON *values = cJSON_CreateArray();
    cJSON_AddItemToObject(settings, "values", values);
    cJSON *n1 = cJSON_CreateNumber(10.0);
    cJSON *n2 = cJSON_CreateNumber(20.0);
    cJSON_AddItemToArray(values, n1);
    cJSON_AddItemToArray(values, n2);
    cJSON *state = cJSON_CreateString("active");
    cJSON_AddItemToObject(settings, "state", state);

    // step 2: Configure
    cJSON *refs = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "refs", refs);
    cJSON *arr_ref = cJSON_CreateArrayReference(values);
    cJSON_InsertItemInArray(refs, 0, arr_ref);
    cJSON *extra = cJSON_CreateNumber(15.0);
    cJSON_InsertItemInArray(values, 1, extra);

    // step 3: Operate & Validate
    cJSON_bool values_is_array = cJSON_IsArray(values);
    cJSON_bool ref_is_array = cJSON_IsArray(arr_ref);
    cJSON_bool first_is_invalid = cJSON_IsInvalid(n1);
    (void)values_is_array;
    (void)ref_is_array;
    (void)first_is_invalid;
    char *printed = cJSON_PrintUnformatted(root);
    size_t printed_len = printed ? strlen(printed) : 0;
    char *buffer = (char *)cJSON_malloc(printed_len + 1);
    memset(buffer, 0, printed_len + 1);
    if (printed_len) memcpy(buffer, printed, printed_len + 1);
    cJSON_Minify(buffer);
    cJSON_free(printed);
    cJSON_free(buffer);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}