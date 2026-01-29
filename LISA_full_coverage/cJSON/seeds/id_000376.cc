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
//<ID> 376
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    int ints[5] = {10, 20, 30, 40, 50};
    cJSON *intArray = cJSON_CreateIntArray(ints, 5);
    cJSON_AddItemToObject(root, "ints", intArray);
    cJSON *raw = cJSON_CreateRaw("{\"note\":\"raw_data\"}");
    cJSON_AddItemToObject(root, "raw_blob", raw);

    // step 2: Configure
    int count = cJSON_GetArraySize(intArray);
    cJSON *third_item = cJSON_GetArrayItem(intArray, 2);
    double third_value = cJSON_GetNumberValue(third_item);
    cJSON_bool raw_flag = cJSON_IsRaw(raw);
    cJSON_AddNumberToObject(root, "int_count", (double)count);
    cJSON_AddNumberToObject(root, "third_value", third_value);
    cJSON_AddNumberToObject(root, "raw_flag", (double)raw_flag);

    // step 3: Operate
    char *printed = cJSON_PrintUnformatted(root);
    int buf_len = 256;
    char *buffer = (char *)cJSON_malloc((size_t)buf_len);
    memset(buffer, 0, (size_t)buf_len);
    cJSON_PrintPreallocated(root, buffer, buf_len, 0);
    cJSON *parsed = cJSON_Parse(printed);
    cJSON *parsed_ints = cJSON_GetObjectItem(parsed, "ints");
    int parsed_count = cJSON_GetArraySize(parsed_ints);
    cJSON *parsed_third = cJSON_GetArrayItem(parsed_ints, 2);
    double parsed_third_value = cJSON_GetNumberValue(parsed_third);
    cJSON_AddNumberToObject(root, "parsed_count", (double)parsed_count);
    cJSON_AddNumberToObject(root, "parsed_third_value", parsed_third_value);

    // step 4: Validate and Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}