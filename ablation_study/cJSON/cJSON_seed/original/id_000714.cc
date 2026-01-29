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
//<ID> 714
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const int numbers[] = {10, 20, 30};
    cJSON *int_array = cJSON_CreateIntArray(numbers, 3);
    cJSON *name_item = cJSON_CreateString("example_name");
    cJSON *root = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "numbers", int_array);
    cJSON_AddItemToObject(root, "name", name_item);

    // step 2: Configure
    int initial_count = cJSON_GetArraySize(int_array);
    cJSON_AddNumberToObject(root, "initial_count", (double)initial_count);
    const char json_text[] = "{\"parsed\":true,\"values\":[7,8,9,10]}";
    const char *parse_end = NULL;
    cJSON *parsed = cJSON_ParseWithLengthOpts(json_text, sizeof(json_text) - 1, &parse_end, 1);
    cJSON *values_arr = cJSON_GetObjectItem(parsed, "values");
    int parsed_values_count = cJSON_GetArraySize(values_arr);
    cJSON_AddNumberToObject(root, "parsed_values_count", (double)parsed_values_count);

    // step 3: Operate
    char buffer[256];
    memset(buffer, 0, sizeof(buffer));
    cJSON_PrintPreallocated(root, buffer, (int)sizeof(buffer), 0);
    char *unformatted = cJSON_PrintUnformatted(root);
    cJSON_free(unformatted);

    // step 4: Validate & Cleanup
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}