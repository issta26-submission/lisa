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
//<ID> 302
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
    cJSON *name = cJSON_CreateString("example");
    cJSON *flag = cJSON_CreateTrue();
    char *printed = NULL;
    cJSON_bool flag_is_bool = 0;
    char *name_value = NULL;
    cJSON *tmp_meta = NULL;
    cJSON *tmp_item = NULL;

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddItemToObject(meta, "name", name);
    cJSON_AddItemToObject(meta, "flag", flag);

    // step 3: Operate / Validate
    printed = cJSON_Print(root);
    cJSON_Minify(printed);
    tmp_meta = cJSON_GetObjectItem(root, "meta");
    tmp_item = cJSON_GetObjectItem(tmp_meta, "name");
    name_value = cJSON_GetStringValue(tmp_item);
    flag_is_bool = cJSON_IsBool(cJSON_GetObjectItem(tmp_meta, "flag"));
    printed[0] = (char)('0' + (int)flag_is_bool);
    printed[1] = name_value[0];

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}