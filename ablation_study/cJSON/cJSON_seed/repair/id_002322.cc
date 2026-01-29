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
//<ID> 2322
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *values = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "values", values);

    // step 2: Configure
    cJSON *sref = cJSON_CreateStringReference("ref-string");
    cJSON_AddItemToArray(values, sref);
    cJSON *num = cJSON_CreateNumber(42.0);
    cJSON_AddItemToArray(values, num);
    cJSON *nested = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "nested", nested);
    cJSON_AddItemToObject(nested, "pi", cJSON_CreateNumber(3.1415));

    // step 3: Operate
    cJSON *values_copy = cJSON_Duplicate(values, 1);
    cJSON_AddItemToObject(root, "values_copy", values_copy);
    char *json = cJSON_PrintUnformatted(root);

    // step 4: Validate and Cleanup
    const int buflen = 128;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    cJSON *first_copy_item = cJSON_GetArrayItem(values_copy, 0);
    const char *first_str = cJSON_GetStringValue(first_copy_item);
    buffer[0] = (json && json[0]) ? json[0] : '\0';
    buffer[1] = (first_str && first_str[0]) ? first_str[0] : '\0';
    cJSON_free(json);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}