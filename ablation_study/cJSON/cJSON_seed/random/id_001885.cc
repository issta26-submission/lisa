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
//<ID> 1885
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
    cJSON *raw = NULL;
    cJSON *false_node = NULL;
    cJSON *got_raw = NULL;
    cJSON *got_flag = NULL;
    char *out = NULL;
    cJSON_bool flag_check = 0;
    int validation = 0;

    // step 2: Setup
    root = cJSON_CreateObject();
    raw = cJSON_CreateRaw("{\"embedded\":true}");
    false_node = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "payload", raw);
    cJSON_AddItemToObject(root, "flag", false_node);

    // step 3: Core operations
    got_raw = cJSON_GetObjectItem(root, "payload");
    got_flag = cJSON_GetObjectItem(root, "flag");
    flag_check = cJSON_IsFalse(got_flag);
    out = cJSON_PrintUnformatted(root);

    // step 4: Validate
    validation ^= (int)(root != NULL);
    validation ^= (int)(raw != NULL);
    validation ^= (int)(false_node != NULL);
    validation ^= (int)(got_raw != NULL);
    validation ^= (int)(got_flag != NULL);
    validation ^= (int)(got_raw == raw);
    validation ^= (int)(got_flag == false_node);
    validation ^= (int)(flag_check != 0);
    validation ^= (int)(out != NULL);
    buf[0] = (char)(validation + '0');

    // step 5: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);

    // step 6: Finalize
    // API sequence test completed successfully
    return 66;
}