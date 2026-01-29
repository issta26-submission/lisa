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
//<ID> 849
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *name_item = NULL;
    cJSON *true_item = NULL;
    cJSON *raw_item = NULL;
    cJSON *got_raw = NULL;
    char *printed = NULL;
    char buffer[256];
    memset(buffer, 0, sizeof(buffer));
    cJSON_bool raw_is_raw = 0;
    const char *name_val = NULL;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    name_item = cJSON_CreateString("example_name");
    cJSON_AddItemToObject(root, "name", name_item);
    true_item = cJSON_AddTrueToObject(root, "enabled");
    raw_item = cJSON_CreateRaw("{\"meta\":true}");
    cJSON_AddItemToObject(root, "meta_raw", raw_item);

    // step 3: Operate / Validate
    got_raw = cJSON_GetObjectItemCaseSensitive(root, "meta_raw");
    raw_is_raw = cJSON_IsRaw(got_raw);
    name_val = cJSON_GetStringValue(name_item);
    printed = cJSON_PrintUnformatted(root);
    buffer[0] = (raw_is_raw ? '1' : '0');
    (void)name_val;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_DeleteItemFromObjectCaseSensitive(root, "name");
    cJSON_Delete(root);
    memset(buffer, 0, sizeof(buffer));

    // API sequence test completed successfully
    return 66;
}