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
//<ID> 731
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json_text = "{\"name\":\"example\",\"arr\":[\"one\",\"two\"],\"num\":42}";
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithOpts(json_text, &parse_end, 1);

    // step 2: Configure
    cJSON *num_item = cJSON_GetObjectItem(root, "num");
    double orig_num = cJSON_GetNumberValue(num_item);
    cJSON_SetNumberHelper(num_item, orig_num + 8.0);
    const char *more_strs[] = { "alpha", "beta", "gamma" };
    cJSON *str_array = cJSON_CreateStringArray(more_strs, 3);
    cJSON_AddItemToObject(root, "new_strs", str_array);

    // step 3: Operate
    cJSON *arr = cJSON_GetObjectItem(root, "arr");
    cJSON *insert_item = cJSON_CreateString("inserted");
    cJSON_InsertItemInArray(arr, 1, insert_item);

    // step 4: Validate & Cleanup
    char *printed = cJSON_PrintUnformatted(root);
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}