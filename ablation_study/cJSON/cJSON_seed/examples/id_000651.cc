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
//<ID> 651
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *version = cJSON_Version();
    cJSON *root = cJSON_CreateObject();
    double initial_numbers[3] = {1.1, 2.2, 3.3};
    cJSON *double_array = cJSON_CreateDoubleArray(initial_numbers, 3);
    cJSON_AddItemToObject(root, "values", double_array);
    cJSON *creator = cJSON_CreateString("tester");
    cJSON_AddItemToObject(root, "creator", creator);

    // step 2: Operate (serialize and minify)
    char *snapshot = cJSON_PrintUnformatted(root);
    size_t snaplen = strlen(snapshot);
    char *buffer = (char *)cJSON_malloc(snaplen + 1);
    memset(buffer, 0, snaplen + 1);
    sprintf(buffer, "%s", snapshot);
    cJSON_Minify(buffer);

    // step 3: Validate and mutate parsed structure
    cJSON *parsed = cJSON_Parse(buffer);
    cJSON *parsed_values = cJSON_GetObjectItem(parsed, "values");
    int original_count = cJSON_GetArraySize(parsed_values);
    double replacement_numbers[2] = {9.9, 8.8};
    cJSON *replacement_array = cJSON_CreateDoubleArray(replacement_numbers, 2);
    cJSON_ReplaceItemInObjectCaseSensitive(parsed, "values", replacement_array);
    cJSON *new_values = cJSON_GetObjectItem(parsed, "values");
    int replaced_count = cJSON_GetArraySize(new_values);
    (void)version;
    (void)original_count;
    (void)replaced_count;

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_free(buffer);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}