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
//<ID> 628
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    char *printed = NULL;
    char *scratch = (char *)cJSON_malloc(512);
    memset(scratch, 0, 512);
    cJSON *root = cJSON_CreateObject();
    cJSON *config = cJSON_AddObjectToObject(root, "config");
    cJSON *items = cJSON_CreateArray();
    cJSON *old_item = cJSON_CreateString("first");
    cJSON *second_item = cJSON_CreateString("second");
    cJSON *replacement = cJSON_CreateString("replaced");
    size_t printed_len = 0;
    size_t to_copy = 0;

    // step 2: Setup / Configure
    cJSON_AddItemToArray(items, old_item);
    cJSON_AddItemToArray(items, second_item);
    cJSON_AddItemToObject(root, "items", items);
    cJSON_AddItemToObject(config, "name", cJSON_CreateString("example"));
    cJSON_AddStringToObject(config, "status", "active");

    // step 3: Operate / Validate
    cJSON *got_config = cJSON_GetObjectItem(root, "config");
    cJSON *got_items = cJSON_GetObjectItem(root, "items");
    cJSON_ReplaceItemInArray(got_items, 0, replacement);
    printed = cJSON_PrintUnformatted(root);
    printed_len = printed ? strlen(printed) : 0;
    to_copy = printed_len < 511 ? printed_len : 511;
    memcpy(scratch, printed ? printed : "", to_copy);
    scratch[to_copy] = '\0';

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(scratch);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}