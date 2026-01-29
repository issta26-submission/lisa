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
//<ID> 1956
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
    cJSON *enabled = NULL;
    cJSON *label = NULL;
    cJSON *got_label = NULL;
    char *got_str = NULL;
    char first_char = '\0';
    cJSON_bool added_enabled = 0;
    cJSON_bool added_label = 0;
    int validation = 0;

    // step 2: Setup
    root = cJSON_CreateObject();
    enabled = cJSON_CreateFalse();
    label = cJSON_CreateString("example");
    added_enabled = cJSON_AddItemToObject(root, "enabled", enabled);
    added_label = cJSON_AddItemToObject(root, "label", label);

    // step 3: Core operations
    got_label = cJSON_GetObjectItem(root, "label");
    got_str = cJSON_GetStringValue(got_label);
    first_char = (got_str != NULL) ? got_str[0] : '\0';

    // step 4: Validate
    validation ^= (int)(root != NULL);
    validation ^= (int)(enabled != NULL);
    validation ^= (int)(label != NULL);
    validation ^= (int)(added_enabled != 0);
    validation ^= (int)(added_label != 0);
    validation ^= (int)(got_label != NULL);
    validation ^= (int)(got_str != NULL);
    validation ^= (int)(first_char == 'e');
    buf[0] = (char)(validation + '0');

    // step 5: Cleanup
    cJSON_Delete(root);

    // step 6: Finalize
    // API sequence test completed successfully
    return 66;
}