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
//<ID> 749
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json_text = "{\"name\":\"test\",\"items\":[1,2,3]}";
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithOpts(json_text, &parse_end, 1);

    // step 2: Configure
    cJSON *items = cJSON_GetObjectItem(root, "items");
    int initial_size = cJSON_GetArraySize(items);
    cJSON *first_elem = cJSON_GetArrayItem(items, 0);
    cJSON *dup_first = cJSON_Duplicate(first_elem, 1);
    cJSON_AddItemToArray(items, dup_first);
    cJSON_AddNullToObject(root, "marker");
    cJSON *flag_false = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "active", flag_false);

    // step 3: Operate
    int final_size = cJSON_GetArraySize(items);
    cJSON_AddNumberToObject(root, "final_count", (double)final_size);
    char *printed = cJSON_PrintUnformatted(root);

    // step 4: Validate & Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}