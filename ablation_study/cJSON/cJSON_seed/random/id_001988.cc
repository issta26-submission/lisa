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
//<ID> 1988
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    char buf[32];
    memset(buf, 0, sizeof(buf));
    cJSON *root = NULL;
    cJSON *child = NULL;
    cJSON *dup_child = NULL;
    cJSON *bool_item = NULL;
    cJSON_bool added_child = 0;
    cJSON_bool replaced = 0;
    cJSON_bool exists_before = 0;
    cJSON_bool exists_after = 0;
    int validation = 0;

    // step 2: Setup
    root = cJSON_CreateObject();
    child = cJSON_CreateObject();
    cJSON_AddNumberToObject(child, "id", 101.0);
    cJSON_AddStringToObject(child, "name", "original");
    added_child = cJSON_AddItemToObject(root, "child", child);

    // step 3: Configure
    exists_before = cJSON_HasObjectItem(root, "child");
    dup_child = cJSON_Duplicate(child, 1);
    bool_item = cJSON_AddBoolToObject(dup_child, "enabled", 1);

    // step 4: Operate
    replaced = cJSON_ReplaceItemViaPointer(root, child, dup_child);
    exists_after = cJSON_HasObjectItem(root, "child");

    // step 5: Validate
    validation ^= (int)(root != NULL);
    validation ^= (int)(child != NULL);
    validation ^= (int)(dup_child != NULL);
    validation ^= (int)(added_child != 0);
    validation ^= (int)(exists_before != 0);
    validation ^= (int)(bool_item != NULL);
    validation ^= (int)(replaced != 0);
    validation ^= (int)(exists_after != 0);
    buf[0] = (char)(validation + '0');

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}