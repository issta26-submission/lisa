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
//<ID> 150
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
    cJSON *title = cJSON_CreateString("FuzzTest");
    cJSON_AddItemToObjectCS(root, "title", title);
    cJSON *author = cJSON_CreateString("tester");
    cJSON_AddItemToObject(root, "author", author);
    cJSON *libver = cJSON_CreateString(cJSON_Version());
    cJSON_AddItemToObject(root, "lib_version", libver);

    // step 3: Operate & Validate
    cJSON *title_item = cJSON_GetObjectItem(root, "title");
    cJSON *title_copy = cJSON_CreateString(cJSON_GetStringValue(title_item));
    cJSON_AddItemToObject(meta, "title_copy", title_copy);
    cJSON *author_item = cJSON_GetObjectItem(root, "author");
    cJSON *author_copy = cJSON_CreateString(cJSON_GetStringValue(author_item));
    cJSON_AddItemToObjectCS(meta, "author_copy", author_copy);
    char *printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}