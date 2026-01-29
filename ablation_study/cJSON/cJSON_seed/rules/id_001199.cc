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
//<ID> 1199
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
    cJSON *name_copy = NULL;
    cJSON *ok = NULL;
    cJSON *retrieved = NULL;
    char *printed = NULL;
    const char *name_val = NULL;
    cJSON_bool has_ok = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    name = cJSON_CreateString("example");
    cJSON_AddItemToObject(root, "name", name);
    ok = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "ok", ok);

    // step 3: Operate / Validate
    has_ok = cJSON_HasObjectItem(root, "ok");
    retrieved = cJSON_GetObjectItem(root, "name");
    name_val = cJSON_GetStringValue(retrieved);
    name_copy = cJSON_CreateString(name_val);
    cJSON_AddItemToObject(root, "name_copy", name_copy);
    printed = cJSON_Print(root);
    (void)has_ok;
    (void)printed;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}