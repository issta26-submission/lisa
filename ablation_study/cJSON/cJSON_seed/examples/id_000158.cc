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
//<ID> 158
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *child = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "child", child);
    cJSON *greeting = cJSON_CreateString("Hello, world");
    cJSON_AddItemToObjectCS(root, "greeting", greeting);
    cJSON *note = cJSON_CreateString("This is a note.");
    cJSON_AddItemToObject(root, "note", note);

    // step 2: Configure
    cJSON *author = cJSON_CreateString("Alice");
    cJSON_AddItemToObject(child, "author", author);
    cJSON *title = cJSON_CreateString("Sample Title");
    cJSON_AddItemToObjectCS(child, "title", title);

    // step 3: Operate & Validate
    cJSON *got_greeting = cJSON_GetObjectItem(root, "greeting");
    char *greet_val = cJSON_GetStringValue(got_greeting);
    cJSON *greeting_copy = cJSON_CreateString(greet_val);
    cJSON_AddItemToObject(root, "greeting_copy", greeting_copy);
    cJSON *got_child = cJSON_GetObjectItem(root, "child");
    cJSON *got_author = cJSON_GetObjectItem(got_child, "author");
    char *author_val = cJSON_GetStringValue(got_author);
    cJSON *author_copy = cJSON_CreateString(author_val);
    cJSON_AddItemToObjectCS(root, "author_copy", author_copy);
    char *printed = cJSON_PrintUnformatted(root);
    cJSON_free(printed);

    // step 4: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}