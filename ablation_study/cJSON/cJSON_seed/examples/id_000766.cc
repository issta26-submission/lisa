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
//<ID> 766
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    double numbers[3] = {3.14, 2.718, 1.618};
    cJSON *values_array = cJSON_CreateDoubleArray(numbers, 3);
    cJSON *nullable_item = cJSON_CreateNull();

    // step 2: Configure
    cJSON_AddItemToObject(root, "values", values_array);
    cJSON_AddItemToObject(root, "nullable", nullable_item);
    cJSON *answer_item = cJSON_AddNumberToObject(root, "answer", 42.0);

    // step 3: Operate & Validate
    char *snapshot = cJSON_PrintUnformatted(root);
    size_t len = strlen(snapshot);
    char *buffer = (char *)cJSON_malloc(len + 1);
    memset(buffer, 0, len + 1);
    memcpy(buffer, snapshot, len + 1);
    cJSON_Minify(buffer);
    cJSON *parsed = cJSON_Parse(buffer);
    cJSON *parsed_nullable = cJSON_GetObjectItem(parsed, "nullable");
    cJSON_bool is_null = cJSON_IsNull(parsed_nullable);
    cJSON *parsed_answer = cJSON_GetObjectItem(parsed, "answer");
    double parsed_value = cJSON_GetNumberValue(parsed_answer);
    (void)is_null;
    (void)parsed_value;
    (void)answer_item;

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_free(buffer);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}