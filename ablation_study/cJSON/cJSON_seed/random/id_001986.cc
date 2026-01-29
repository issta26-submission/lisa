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
//<ID> 1986
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
    cJSON *child = NULL;
    cJSON *dup_child = NULL;
    cJSON *replacement = NULL;
    cJSON *bool_item = NULL;
    cJSON_bool added = 0;
    cJSON_bool has_before = 0;
    cJSON_bool replaced = 0;
    cJSON_bool has_flag = 0;
    int validation = 0;

    // step 2: Setup
    root = cJSON_CreateObject();
    child = cJSON_CreateString("one");
    added = cJSON_AddItemToObject(root, "child", child);

    // step 3: Core operations
    has_before = cJSON_HasObjectItem(root, "child");
    dup_child = cJSON_Duplicate(child, 1);
    replacement = cJSON_CreateString("two");
    replaced = cJSON_ReplaceItemViaPointer(root, child, replacement);
    bool_item = cJSON_AddBoolToObject(root, "flag", 1);
    has_flag = cJSON_HasObjectItem(root, "flag");

    // step 4: Validate
    validation ^= (int)(root != NULL);
    validation ^= (int)(child != NULL);
    validation ^= (int)(dup_child != NULL);
    validation ^= (int)(replacement != NULL);
    validation ^= (int)(added != 0);
    validation ^= (int)(has_before != 0);
    validation ^= (int)(replaced != 0);
    validation ^= (int)(bool_item != NULL);
    validation ^= (int)(has_flag != 0);
    buf[0] = (char)(validation + '0');

    // step 5: Cleanup
    cJSON_Delete(root);
    cJSON_Delete(dup_child);

    // step 6: Finalize
    // API sequence test completed successfully
    return 66;
}