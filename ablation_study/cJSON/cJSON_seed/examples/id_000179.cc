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
//<ID> 179
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    const char *words[] = { "alpha", "beta", "gamma" };
    cJSON *word_array = cJSON_CreateStringArray(words, 3);
    cJSON_AddItemToObject(meta, "words", word_array);
    cJSON *title = cJSON_CreateString("original_title");
    cJSON_AddItemToObject(root, "title", title);

    // step 2: Configure
    cJSON *new_title = cJSON_CreateString("updated_title");
    cJSON_ReplaceItemInObject(root, "title", new_title);

    // step 3: Operate
    cJSON *got_title = cJSON_GetObjectItem(root, "title");
    char *title_val = cJSON_GetStringValue(got_title);
    char buffer[256];
    memset(buffer, 0, sizeof(buffer));
    cJSON_PrintPreallocated(root, buffer, (int)sizeof(buffer), 0);

    // step 4: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}