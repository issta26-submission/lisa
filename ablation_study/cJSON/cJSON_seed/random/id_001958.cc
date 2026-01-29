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
//<ID> 1958
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
    cJSON *name = NULL;
    cJSON *status = NULL;
    cJSON *meta = NULL;
    cJSON *note = NULL;
    cJSON *got_name_item = NULL;
    char *got_name = NULL;
    cJSON_bool added_name = 0;
    cJSON_bool added_status = 0;
    cJSON_bool added_meta = 0;
    cJSON_bool added_note = 0;
    int validation = 0;

    // step 2: Setup
    root = cJSON_CreateObject();
    name = cJSON_CreateString("example");
    status = cJSON_CreateFalse();
    added_name = cJSON_AddItemToObject(root, "name", name);
    added_status = cJSON_AddItemToObject(root, "status", status);

    // step 3: Configure
    meta = cJSON_CreateObject();
    added_meta = cJSON_AddItemToObject(root, "meta", meta);
    note = cJSON_CreateString("metadata");
    added_note = cJSON_AddItemToObject(meta, "note", note);

    // step 4: Operate
    got_name_item = cJSON_GetObjectItem(root, "name");
    got_name = cJSON_GetStringValue(got_name_item);

    // step 5: Validate
    validation ^= (int)(root != NULL);
    validation ^= (int)(name != NULL);
    validation ^= (int)(status != NULL);
    validation ^= (int)(meta != NULL);
    validation ^= (int)(note != NULL);
    validation ^= (int)(added_name != 0);
    validation ^= (int)(added_status != 0);
    validation ^= (int)(added_meta != 0);
    validation ^= (int)(added_note != 0);
    validation ^= (int)(got_name_item != NULL);
    validation ^= (int)(got_name != NULL);
    buf[0] = (char)(validation + '0');

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}