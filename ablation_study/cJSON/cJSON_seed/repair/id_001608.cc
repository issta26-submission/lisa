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
//<ID> 1608
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize and Configure
    cJSON *root = cJSON_CreateObject();
    cJSON *true_item = cJSON_CreateTrue();
    cJSON *false_item = cJSON_CreateBool((cJSON_bool)0);
    cJSON_AddItemToObject(root, "flag_true", true_item);
    cJSON_AddItemToObject(root, "flag_false", false_item);
    cJSON *derived_item = cJSON_CreateBool(cJSON_IsTrue(true_item));
    cJSON_AddItemToObject(root, "derived_from_true", derived_item);

    // step 2: Operate
    const char *version = cJSON_Version();
    char *unformatted = cJSON_PrintUnformatted((const cJSON *)root);

    // step 3: Validate
    cJSON *got_true = cJSON_GetObjectItem(root, "flag_true");
    cJSON *got_false = cJSON_GetObjectItem(root, "flag_false");
    cJSON_bool got_true_is_true = cJSON_IsTrue(got_true);
    cJSON_bool got_false_is_bool = cJSON_IsBool(got_false);
    char buffer[64];
    memset(buffer, 0, sizeof(buffer));
    buffer[0] = version ? version[0] : '0';
    buffer[1] = (char)('0' + (int)got_true_is_true);
    buffer[2] = (char)('0' + (int)got_false_is_bool);
    buffer[3] = unformatted ? unformatted[0] : '\0';

    // step 4: Cleanup
    if (unformatted) cJSON_free(unformatted);
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}