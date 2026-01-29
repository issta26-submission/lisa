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
//<ID> 1889
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
    cJSON *raw_item = NULL;
    cJSON *false_item = NULL;
    cJSON *got_false = NULL;
    cJSON *got_raw = NULL;
    char *out = NULL;
    cJSON_bool is_false = 0;
    int validation = 0;

    // step 2: Setup
    root = cJSON_CreateObject();
    raw_item = cJSON_CreateRaw("{\"inner\":123}");
    cJSON_AddItemToObject(root, "raw", raw_item);
    false_item = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "enabled", false_item);

    // step 3: Core operations
    got_false = cJSON_GetObjectItem(root, "enabled");
    is_false = cJSON_IsFalse(got_false);
    got_raw = cJSON_GetObjectItem(root, "raw");
    out = cJSON_PrintUnformatted(root);

    // step 4: Validate
    validation ^= (int)(root != NULL);
    validation ^= (int)(raw_item != NULL);
    validation ^= (int)(false_item != NULL);
    validation ^= (int)(got_false != NULL);
    validation ^= (int)(is_false != 0);
    validation ^= (int)(got_raw != NULL);
    validation ^= (int)(out != NULL);
    buf[0] = (char)(validation + '0');

    // step 5: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);

    // step 6: Finalize
    // API sequence test completed successfully
    return 66;
}