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
//<ID> 216
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *names[] = { "Alice", "Bob", "Charlie" };
    cJSON *root = cJSON_CreateObject();
    cJSON *names_array = cJSON_CreateStringArray(names, 3);
    cJSON_AddItemToObject(root, "names", names_array);
    cJSON *count = cJSON_CreateNumber(3.0);
    cJSON_AddItemToObject(root, "count", count);

    // step 2: Configure
    double newcount = cJSON_SetNumberHelper(count, 42.0);
    cJSON *extra = cJSON_CreateNumber(0.0);
    cJSON_SetNumberHelper(extra, newcount + 1.0);
    cJSON_AddItemToObject(root, "new_count", extra);
    cJSON *dup_root = cJSON_Duplicate(root, 1);

    // step 3: Operate and Validate
    cJSON *first_name = cJSON_GetArrayItem(names_array, 0);
    cJSON *detached = cJSON_DetachItemViaPointer(names_array, first_name);
    cJSON_AddItemToObject(root, "first_detached", detached);
    char *printed = cJSON_PrintUnformatted(root);
    cJSON_Minify(printed);
    void *tmp_mem = cJSON_malloc(64);
    memset(tmp_mem, 0, 64);
    cJSON *parsed = cJSON_Parse(printed);
    cJSON *dup_of_parsed = cJSON_Duplicate(parsed, 0);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(tmp_mem);
    cJSON_Delete(parsed);
    cJSON_Delete(dup_of_parsed);
    cJSON_Delete(dup_root);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}