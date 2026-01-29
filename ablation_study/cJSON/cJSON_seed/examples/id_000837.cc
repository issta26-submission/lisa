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
//<ID> 837
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    double initial_numbers[3] = { 1.1, 2.2, 3.3 };
    cJSON *numbers = cJSON_CreateDoubleArray(initial_numbers, 3);
    cJSON *raw_replacement = cJSON_CreateRaw("\"replaced\"");
    cJSON_AddItemToObjectCS(root, "numbers", numbers);

    // step 2: Configure
    cJSON_ReplaceItemInArray(numbers, 1, raw_replacement);

    // step 3: Operate & Validate
    char *snapshot = cJSON_PrintUnformatted(root);
    size_t len = strlen(snapshot);
    char *buffer = (char *)cJSON_malloc(len + 1);
    memset(buffer, 0, len + 1);
    memcpy(buffer, snapshot, len + 1);
    cJSON_Minify(buffer);
    cJSON *parsed = cJSON_Parse(buffer);
    cJSON *parsed_numbers = cJSON_GetObjectItem(parsed, "numbers");
    cJSON *first = cJSON_GetArrayItem(parsed_numbers, 0);
    cJSON *third = cJSON_GetArrayItem(parsed_numbers, 2);
    double sum_edges = cJSON_GetNumberValue(first) + cJSON_GetNumberValue(third);
    double summary_vals[2] = { sum_edges, (double)cJSON_GetArraySize(parsed_numbers) };
    cJSON *summary = cJSON_CreateDoubleArray(summary_vals, 2);
    cJSON_AddItemToObjectCS(parsed, "summary", summary);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_free(buffer);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}