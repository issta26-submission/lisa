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
//<ID> 785
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    double numbers[4] = {0.1, 2.0, -3.14, 4.25};
    cJSON *numbers_array = cJSON_CreateDoubleArray(numbers, 4);
    cJSON *id_string = cJSON_CreateString("sensorA");

    // step 2: Configure
    cJSON_AddItemToObject(root, "id", id_string);
    cJSON_AddItemToObject(root, "values", numbers_array);

    // step 3: Operate & Validate
    char *snapshot = cJSON_PrintUnformatted(root);
    size_t len = strlen(snapshot);
    char *buffer = (char *)cJSON_malloc(len + 1);
    memset(buffer, 0, len + 1);
    memcpy(buffer, snapshot, len + 1);
    cJSON_Minify(buffer);
    cJSON *parsed = cJSON_Parse(buffer);
    cJSON *parsed_values = cJSON_GetObjectItem(parsed, "values");
    int parsed_size = cJSON_GetArraySize(parsed_values);
    cJSON *first_item = cJSON_GetArrayItem(parsed_values, 0);
    double first_value = cJSON_GetNumberValue(first_item);
    cJSON *duplicate_root = cJSON_Duplicate(root, 1);
    cJSON_bool equal = cJSON_Compare(root, duplicate_root, 1);
    char *dup_snapshot = cJSON_PrintUnformatted(duplicate_root);
    (void)parsed_size;
    (void)first_value;
    (void)equal;

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_free(dup_snapshot);
    cJSON_free(buffer);
    cJSON_Delete(parsed);
    cJSON_Delete(duplicate_root);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}