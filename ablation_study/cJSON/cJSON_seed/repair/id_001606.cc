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
//<ID> 1606
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *true_item = cJSON_CreateTrue();
    cJSON *bool_item = cJSON_CreateBool(0);

    // step 2: Configure
    cJSON_AddItemToObject(root, "truth", true_item);
    cJSON_AddItemToObject(root, "falsehood", bool_item);
    const char *version = cJSON_Version();

    // step 3: Operate and Validate
    char buffer[64];
    memset(buffer, 0, sizeof(buffer));
    char *json_unformatted = cJSON_PrintUnformatted(root);
    cJSON *got_truth = cJSON_GetObjectItem(root, "truth");
    cJSON *got_falsehood = cJSON_GetObjectItem(root, "falsehood");
    cJSON_bool truth_flag = cJSON_IsTrue(got_truth);
    cJSON_bool false_flag = cJSON_IsFalse(got_falsehood);
    buffer[0] = version[0];
    buffer[1] = (char)('0' + (int)truth_flag);
    buffer[2] = (char)('0' + (int)false_flag);
    buffer[3] = json_unformatted[0];

    // step 4: Cleanup
    cJSON_free(json_unformatted);
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}