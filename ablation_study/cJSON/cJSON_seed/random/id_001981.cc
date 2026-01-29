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
//<ID> 1981
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    char buf[48];
    memset(buf, 0, sizeof(buf));
    cJSON *root = NULL;
    cJSON *flag_item = NULL;
    cJSON *other_item = NULL;
    cJSON *dup_item = NULL;
    cJSON *replacement = NULL;
    cJSON *got_other = NULL;
    cJSON *got_flag = NULL;
    cJSON_bool has_flag_before = 0;
    cJSON_bool has_other_before = 0;
    cJSON_bool added_other = 0;
    cJSON_bool added_dup = 0;
    cJSON_bool replaced = 0;
    int validation = 0;

    // step 2: Setup
    root = cJSON_CreateObject();
    flag_item = cJSON_AddBoolToObject(root, "flag", 1);
    other_item = cJSON_CreateFalse();
    added_other = cJSON_AddItemToObject(root, "other_flag", other_item);

    // step 3: Core operations
    has_flag_before = cJSON_HasObjectItem(root, "flag");
    has_other_before = cJSON_HasObjectItem(root, "other_flag");
    got_other = cJSON_GetObjectItem(root, "other_flag");
    dup_item = cJSON_Duplicate(got_other, 0);
    added_dup = cJSON_AddItemToObject(root, "dup_flag", dup_item);
    replacement = cJSON_CreateTrue();
    got_flag = cJSON_GetObjectItem(root, "flag");
    replaced = cJSON_ReplaceItemViaPointer(root, got_flag, replacement);

    // step 4: Validate
    validation ^= (int)(root != NULL);
    validation ^= (int)(flag_item != NULL);
    validation ^= (int)(other_item != NULL);
    validation ^= (int)(dup_item != NULL);
    validation ^= (int)(added_other != 0);
    validation ^= (int)(added_dup != 0);
    validation ^= (int)(has_flag_before != 0);
    validation ^= (int)(has_other_before != 0);
    validation ^= (int)(replaced != 0);
    buf[0] = (char)(validation + '0');

    // step 5: Cleanup
    cJSON_Delete(root);

    // step 6: Finalize
    // API sequence test completed successfully
    return 66;
}