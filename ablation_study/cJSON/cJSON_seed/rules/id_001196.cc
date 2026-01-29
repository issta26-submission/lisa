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
//<ID> 1196
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *name = NULL;
    cJSON *false_item = NULL;
    char *printed = NULL;
    char *name_val = NULL;
    cJSON_bool has_name = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    name = cJSON_CreateString("example");
    cJSON_AddItemToObject(root, "name", name);
    false_item = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "enabled", false_item);

    // step 3: Operate / Validate
    name_val = cJSON_GetStringValue(cJSON_GetObjectItem(root, "name"));
    has_name = cJSON_HasObjectItem(root, "name");
    printed = cJSON_Print(root);
    (void)name_val;
    (void)has_name;
    (void)printed;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}