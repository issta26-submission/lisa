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
//<ID> 151
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

    // step 2: Configure
    cJSON *greeting = cJSON_CreateString("Hello, world!");
    cJSON_AddItemToObjectCS(root, "greeting", greeting);
    cJSON *title = cJSON_CreateString("Engineer");
    cJSON_AddItemToObject(root, "title", title);

    // step 3: Operate & Validate
    cJSON *got_greeting = cJSON_GetObjectItem(root, "greeting");
    char *gval = cJSON_GetStringValue(got_greeting);
    cJSON *gcopy = cJSON_CreateString(gval);
    cJSON_AddItemToObjectCS(root, "greeting_copy", gcopy);
    cJSON *got_title = cJSON_GetObjectItem(root, "title");
    char *tval = cJSON_GetStringValue(got_title);
    cJSON *tcopy = cJSON_CreateString(tval);
    cJSON_AddItemToObject(root, "title_copy", tcopy);
    char *printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}