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
//<ID> 306
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_CreateObject();
    cJSON *name_item = cJSON_CreateString("example_name");
    cJSON *active_item = cJSON_CreateTrue();
    cJSON *value_item = cJSON_CreateNumber(42.0);
    char *printed = NULL;
    char *scratch = (char *)cJSON_malloc(256);
    memset(scratch, 0, 256);
    char *name_str = NULL;
    cJSON_bool is_bool = 0;

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "name", name_item);
    cJSON_AddItemToObject(root, "active", active_item);
    cJSON_AddItemToObject(meta, "value", value_item);
    cJSON_AddItemToObject(root, "meta", meta);

    // step 3: Operate / Validate
    printed = cJSON_PrintUnformatted(root);
    cJSON_Minify(printed);
    name_item = cJSON_GetObjectItem(root, "name");
    name_str = cJSON_GetStringValue(name_item);
    active_item = cJSON_GetObjectItem(root, "active");
    is_bool = cJSON_IsBool(active_item);
    scratch[0] = (char)('0' + (int)is_bool);
    scratch[1] = name_str[0];
    scratch[2] = '\0';
    (void)printed;
    (void)name_str;
    (void)is_bool;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(scratch);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}