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
//<ID> 119
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *greet = cJSON_CreateString("hello");
    cJSON *num = cJSON_CreateNumber(123.0);
    cJSON *replacement = cJSON_CreateString("world");
    cJSON *found = NULL;
    char *printed = NULL;
    cJSON_bool replaced = 0;

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "greeting", greet);
    cJSON_AddItemToObject(root, "count", num);

    // step 3: Operate / Validate
    found = cJSON_GetObjectItem(root, "greeting");
    replaced = cJSON_ReplaceItemViaPointer(root, found, replacement);
    printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}