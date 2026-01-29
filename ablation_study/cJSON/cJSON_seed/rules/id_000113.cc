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
//<ID> 113
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *item_a = cJSON_CreateString("initialA");
    cJSON *item_b = cJSON_CreateString("initialB");
    cJSON *replacement = cJSON_CreateNumber(3.14159);
    cJSON *fetched = NULL;
    char *printed = NULL;
    cJSON_bool replaced = 0;

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "a", item_a);
    cJSON_AddItemToObject(root, "b", item_b);

    // step 3: Operate / Validate
    fetched = cJSON_GetObjectItem(root, "a");
    replaced = cJSON_ReplaceItemViaPointer(root, fetched, replacement);
    printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}