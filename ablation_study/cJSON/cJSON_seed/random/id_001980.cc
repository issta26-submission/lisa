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
//<ID> 1980
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
    cJSON *orig_item = NULL;
    cJSON *dup_item = NULL;
    cJSON *replacement = NULL;
    cJSON *flag_item = NULL;
    cJSON_bool added_orig = 0;
    cJSON_bool replaced = 0;
    cJSON_bool has_key = 0;
    cJSON_bool has_flag = 0;
    int validation = 0;

    // step 2: Setup
    root = cJSON_CreateObject();
    orig_item = cJSON_CreateString("original_value");
    added_orig = cJSON_AddItemToObject(root, "key", orig_item);
    flag_item = cJSON_AddBoolToObject(root, "flag", 1);

    // step 3: Core operations
    dup_item = cJSON_Duplicate(orig_item, 1);
    replacement = cJSON_CreateString("replaced_value");
    replaced = cJSON_ReplaceItemViaPointer(root, orig_item, replacement);
    cJSON_AddItemToObject(root, "duplicate", dup_item);
    has_key = cJSON_HasObjectItem(root, "key");
    has_flag = cJSON_HasObjectItem(root, "flag");

    // step 4: Validate
    validation ^= (int)(root != NULL);
    validation ^= (int)(orig_item != NULL);
    validation ^= (int)(dup_item != NULL);
    validation ^= (int)(replacement != NULL);
    validation ^= (int)(added_orig != 0);
    validation ^= (int)(replaced != 0);
    validation ^= (int)(has_key != 0);
    validation ^= (int)(has_flag != 0);
    buf[0] = (char)(validation + '0');

    // step 5: Cleanup
    cJSON_Delete(root);

    // step 6: Finalize
    // API sequence test completed successfully
    return 66;
}