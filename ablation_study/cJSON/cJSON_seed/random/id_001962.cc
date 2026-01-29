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
//<ID> 1962
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
    cJSON *num_item = NULL;
    cJSON *str_item = NULL;
    cJSON *got_num = NULL;
    cJSON *got_str = NULL;
    cJSON *bool_item = NULL;
    char *copied_str = NULL;
    const char *orig_str = NULL;
    double num_val = 0.0;
    int validation = 0;

    // step 2: Setup
    root = cJSON_CreateObject();
    num_item = cJSON_CreateNumber(42.0);
    str_item = cJSON_CreateString("example");
    cJSON_AddItemToObject(root, "answer", num_item);
    cJSON_AddItemToObject(root, "text", str_item);
    bool_item = cJSON_AddBoolToObject(root, "ok", 1);

    // step 3: Core operations
    got_num = cJSON_GetObjectItem(root, "answer");
    num_val = cJSON_GetNumberValue(got_num);
    got_str = cJSON_GetObjectItem(root, "text");
    orig_str = cJSON_GetStringValue(got_str);
    copied_str = (char *)cJSON_malloc((orig_str != NULL ? (strlen(orig_str) + 1) : 1));
    memcpy(copied_str, (orig_str != NULL ? orig_str : ""), (orig_str != NULL ? (strlen(orig_str) + 1) : 1));

    // step 4: Validate
    validation ^= (int)(root != NULL);
    validation ^= (int)(num_item != NULL);
    validation ^= (int)(str_item != NULL);
    validation ^= (int)(bool_item != NULL);
    validation ^= (int)(got_num != NULL);
    validation ^= (int)(got_str != NULL);
    validation ^= (int)(num_val == 42.0);
    validation ^= (int)(copied_str != NULL);
    buf[0] = (char)(validation + '0');

    // step 5: Cleanup
    cJSON_free(copied_str);
    cJSON_Delete(root);

    // step 6: Finalize
    // API sequence test completed successfully
    return 66;
}