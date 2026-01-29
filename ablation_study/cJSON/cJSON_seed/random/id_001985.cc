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
//<ID> 1985
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
    cJSON *inner_num = NULL;
    cJSON *dup_child = NULL;
    cJSON_bool has_before = 0;
    cJSON_bool has_after = 0;
    cJSON *added_flag = NULL;
    cJSON_bool replaced = 0;
    int validation = 0;

    // step 2: Setup
    root = cJSON_CreateObject();
    child = cJSON_CreateObject();
    inner_num = cJSON_CreateNumber(7.0);
    cJSON_AddItemToObject(child, "val", inner_num);
    cJSON_AddItemToObject(root, "node", child);

    // step 3: Core operations
    has_before = cJSON_HasObjectItem(root, "node");
    dup_child = cJSON_Duplicate(child, 1);
    added_flag = cJSON_AddBoolToObject(root, "active", 1);
    replaced = cJSON_ReplaceItemViaPointer(root, child, dup_child);
    has_after = cJSON_HasObjectItem(root, "node");

    // step 4: Validate
    validation ^= (int)(root != NULL);
    validation ^= (int)(child != NULL);
    validation ^= (int)(inner_num != NULL);
    validation ^= (int)(dup_child != NULL);
    validation ^= (int)(added_flag != NULL);
    validation ^= (int)(has_before != 0);
    validation ^= (int)(replaced != 0);
    validation ^= (int)(has_after != 0);
    buf[0] = (char)(validation + '0');

    // step 5: Cleanup
    cJSON_Delete(root);

    // step 6: Finalize
    // API sequence test completed successfully
    return 66;
}