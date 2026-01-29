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
//<ID> 767
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    double numbers[3] = {1.1, 2.2, 3.3};
    cJSON *values_array = cJSON_CreateDoubleArray(numbers, 3);
    cJSON *null_item = cJSON_CreateNull();
    cJSON *pi_item = cJSON_AddNumberToObject(root, "pi", 3.14159);

    // step 2: Configure
    cJSON_AddItemToObject(root, "values", values_array);
    cJSON_AddItemToObject(root, "optional", null_item);
    (void)pi_item;

    // step 3: Operate & Validate
    char *snapshot = cJSON_PrintUnformatted(root);
    size_t len = 0;
    if (snapshot) { len = strlen(snapshot); }
    char *buffer = (char *)cJSON_malloc(len + 1);
    memset(buffer, 0, len + 1);
    if (snapshot) { memcpy(buffer, snapshot, len + 1); }
    cJSON_Minify(buffer);
    cJSON *parsed = cJSON_Parse(buffer);
    cJSON *parsed_optional = cJSON_GetObjectItem(parsed, "optional");
    cJSON_bool optional_is_null = cJSON_IsNull(parsed_optional);
    cJSON *parsed_values = cJSON_GetObjectItem(parsed, "values");
    int values_count = cJSON_GetArraySize(parsed_values);
    cJSON *parsed_pi = cJSON_GetObjectItem(parsed, "pi");
    double pi_value = cJSON_GetNumberValue(parsed_pi);
    (void)optional_is_null;
    (void)values_count;
    (void)pi_value;

    // step 4: Cleanup
    if (snapshot) { cJSON_free(snapshot); }
    if (buffer) { cJSON_free(buffer); }
    if (parsed) { cJSON_Delete(parsed); }
    if (root) { cJSON_Delete(root); }

    // API sequence test completed successfully
    return 66;
}