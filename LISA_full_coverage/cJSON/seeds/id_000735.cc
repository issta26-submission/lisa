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
//<ID> 735
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json_text = "{\"title\":\"example\",\"numbers\":[1,2],\"names\":[\"alice\",\"bob\"]}";
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithOpts(json_text, &parse_end, 1);

    // step 2: Configure
    cJSON *numbers = cJSON_GetObjectItem(root, "numbers");
    cJSON *first_num = cJSON_GetArrayItem(numbers, 0);
    double orig_val = cJSON_GetNumberValue(first_num);
    cJSON_SetNumberHelper(first_num, orig_val + 3.5);
    const char *extra_names[] = { "carol", "dave" };
    cJSON *extra_arr = cJSON_CreateStringArray(extra_names, 2);

    // step 3: Operate
    cJSON *names = cJSON_GetObjectItem(root, "names");
    cJSON *extra_first = extra_arr->child;
    cJSON *extra_copy = cJSON_Duplicate(extra_first, 1);
    cJSON_InsertItemInArray(names, 1, extra_copy);
    char *printed = cJSON_PrintUnformatted(root);

    // step 4: Validate & Cleanup
    cJSON_free(printed);
    cJSON_Delete(extra_arr);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}