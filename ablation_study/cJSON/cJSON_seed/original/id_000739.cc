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
//<ID> 739
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json_text = "{\"title\":\"example\",\"values\":[\"one\",\"two\"],\"count\":2}";
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithOpts(json_text, &parse_end, 1);

    // step 2: Configure
    cJSON *count_item = cJSON_GetObjectItem(root, "count");
    double count_val = cJSON_GetNumberValue(count_item);
    cJSON_SetNumberHelper(count_item, count_val + 1.0);
    const char *new_strings[] = { "three", "four" };
    cJSON *new_arr = cJSON_CreateStringArray(new_strings, 2);

    // step 3: Operate
    cJSON *values = cJSON_GetObjectItem(root, "values");
    int insert_index = cJSON_GetArraySize(values);
    cJSON_InsertItemInArray(values, insert_index, new_arr);

    // step 4: Validate & Cleanup
    int final_size = cJSON_GetArraySize(values);
    cJSON_AddItemToObject(root, "final_size", cJSON_CreateNumber((double)final_size));
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}