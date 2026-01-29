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
//<ID> 178
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    const char *initial_strings[] = { "alpha", "beta", "gamma" };
    cJSON *items = cJSON_CreateStringArray(initial_strings, 3);
    cJSON_AddItemToObject(root, "items", items);
    cJSON *old_title = cJSON_CreateString("old_title");
    cJSON_AddItemToObject(root, "title", old_title);

    // step 2: Configure
    cJSON *new_title = cJSON_CreateString("final_title");
    cJSON_ReplaceItemInObject(root, "title", new_title);
    cJSON *extra_note = cJSON_CreateString("note");
    cJSON_AddItemToObject(root, "note", extra_note);

    // step 3: Operate & Validate
    cJSON *got_title = cJSON_GetObjectItem(root, "title");
    char *title_val = cJSON_GetStringValue(got_title);
    cJSON *got_items = cJSON_GetObjectItem(root, "items");
    cJSON *first_item = cJSON_GetArrayItem(got_items, 0);
    char *first_val = cJSON_GetStringValue(first_item);
    const char *summary_strings[] = { title_val, first_val };
    cJSON *summary = cJSON_CreateStringArray(summary_strings, 2);
    cJSON_AddItemToObject(root, "summary", summary);
    char buffer[512];
    memset(buffer, 0, sizeof(buffer));
    cJSON_PrintPreallocated(root, buffer, (int)sizeof(buffer), 1);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}