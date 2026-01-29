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
//<ID> 2047
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    char buf[64];
    memset(buf, 0, sizeof(buf));
    cJSON *root = NULL;
    cJSON *str_item = NULL;
    cJSON *manual_bool = NULL;
    cJSON *auto_bool = NULL;
    cJSON *got_str_obj = NULL;
    cJSON *got_manual = NULL;
    cJSON *got_auto = NULL;
    char *str_val = NULL;
    cJSON_bool manual_invalid = 0;
    cJSON_bool auto_invalid = 0;
    void *mem = NULL;
    int validation = 0;

    // step 2: Setup
    root = cJSON_CreateObject();
    str_item = cJSON_CreateString("hello");
    manual_bool = cJSON_CreateTrue();
    mem = cJSON_malloc(32);
    memset(mem, 'A', 32);

    // step 3: Configure
    cJSON_AddItemToObject(root, "greeting", str_item);
    cJSON_AddItemToObject(root, "manual_bool", manual_bool);
    auto_bool = cJSON_AddBoolToObject(root, "auto_bool", 1);

    // step 4: Operate / Inspect
    got_str_obj = cJSON_GetObjectItem(root, "greeting");
    str_val = cJSON_GetStringValue(got_str_obj);
    got_manual = cJSON_GetObjectItem(root, "manual_bool");
    got_auto = cJSON_GetObjectItem(root, "auto_bool");
    manual_invalid = cJSON_IsInvalid(got_manual);
    auto_invalid = cJSON_IsInvalid(got_auto);

    // step 5: Validate
    validation ^= (int)(root != NULL);
    validation ^= (int)(str_item != NULL);
    validation ^= (int)(manual_bool != NULL);
    validation ^= (int)(auto_bool != NULL);
    validation ^= (int)(mem != NULL);
    validation ^= (int)(str_val != NULL);
    validation ^= (int)(manual_invalid == 0);
    validation ^= (int)(auto_invalid == 0);
    buf[0] = (char)(validation + '0');

    // step 6: Cleanup
    cJSON_Delete(root);
    cJSON_free(mem);

    // API sequence test completed successfully
    return 66;
}