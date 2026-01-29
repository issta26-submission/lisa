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
//<ID> 1264
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    const char raw_json[] = "{\"meta\":123}";
    cJSON *root = cJSON_CreateObject();
    cJSON *raw_item = NULL;
    cJSON *nested = NULL;
    cJSON *flag = NULL;
    cJSON *got_raw = NULL;
    char *printed1 = NULL;
    char *printed2 = NULL;
    char buffer[256];
    cJSON_bool add_ok = 0;
    cJSON_bool prealloc_ok = 0;
    cJSON_bool flag_is_true = 0;
    const char *raw_str = NULL;
    memset(buffer, 0, sizeof(buffer));

    // step 2: Setup / Configure
    raw_item = cJSON_CreateRaw(raw_json);
    add_ok = cJSON_AddItemToObjectCS(root, "rawdata", raw_item);
    nested = cJSON_AddObjectToObject(root, "nested");
    flag = cJSON_AddTrueToObject(nested, "enabled");

    // step 3: Operate / Validate
    got_raw = cJSON_GetObjectItem(root, "rawdata");
    raw_str = cJSON_GetStringValue(got_raw);
    flag_is_true = cJSON_IsTrue(flag);
    printed1 = cJSON_PrintUnformatted(root);
    prealloc_ok = cJSON_PrintPreallocated(root, buffer, (int)sizeof(buffer), 0);
    printed2 = cJSON_Print(root);
    cJSON_free(printed1);
    cJSON_free(printed2);
    (void)raw_str;
    (void)add_ok;
    (void)prealloc_ok;
    (void)flag_is_true;

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}