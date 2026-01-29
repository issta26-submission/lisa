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
//<ID> 1145
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *old_item = NULL;
    cJSON *replacement = NULL;
    cJSON *fetched = NULL;
    char *dynamic_str = NULL;
    char *printed = NULL;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    old_item = cJSON_CreateString("old_value");
    cJSON_AddItemToObject(root, "status", old_item);
    dynamic_str = (char *)cJSON_malloc(8);
    memset(dynamic_str, 0, 8);
    dynamic_str[0] = 'n';
    dynamic_str[1] = 'e';
    dynamic_str[2] = 'w';
    dynamic_str[3] = '\0';
    replacement = cJSON_CreateString(dynamic_str);

    // step 3: Operate / Validate
    cJSON_ReplaceItemViaPointer(root, old_item, replacement);
    fetched = cJSON_GetObjectItem(root, "status");
    (void)cJSON_IsString(fetched);
    printed = cJSON_Print(root);

    // step 4: Cleanup
    cJSON_Delete(root);
    cJSON_free(printed);
    cJSON_free(dynamic_str);
    // API sequence test completed successfully
    return 66;
}