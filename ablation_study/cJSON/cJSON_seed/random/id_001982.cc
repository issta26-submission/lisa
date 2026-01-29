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
//<ID> 1982
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
    cJSON *dup_item = NULL;
    cJSON *replacement = NULL;
    cJSON *flag_item = NULL;
    cJSON_bool added = 0;
    cJSON_bool has_before = 0;
    cJSON_bool replace_ok = 0;
    cJSON_bool has_after = 0;
    cJSON_bool has_dup = 0;
    cJSON_bool has_flag = 0;
    int validation = 0;

    // step 2: Setup
    root = cJSON_CreateObject();
    num_item = cJSON_CreateNumber(10.0);
    added = cJSON_AddItemToObject(root, "num", num_item);
    has_before = cJSON_HasObjectItem(root, "num");
    dup_item = cJSON_Duplicate(num_item, 1);

    // step 3: Core operations
    replacement = cJSON_CreateNumber(20.0);
    replace_ok = cJSON_ReplaceItemViaPointer(root, num_item, replacement);
    flag_item = cJSON_AddBoolToObject(root, "flag", 1);
    cJSON_AddItemToObject(root, "dup_num", dup_item);

    // step 4: Validate
    has_after = cJSON_HasObjectItem(root, "num");
    has_dup = cJSON_HasObjectItem(root, "dup_num");
    has_flag = cJSON_HasObjectItem(root, "flag");
    validation ^= (int)(root != NULL);
    validation ^= (int)(num_item != NULL);
    validation ^= (int)(added != 0);
    validation ^= (int)(has_before != 0);
    validation ^= (int)(replace_ok != 0);
    validation ^= (int)(has_after != 0);
    validation ^= (int)(has_dup != 0);
    validation ^= (int)(has_flag != 0);
    buf[0] = (char)(validation + '0');

    // step 5: Cleanup
    cJSON_Delete(root);

    // step 6: Finalize
    // API sequence test completed successfully
    return 66;
}