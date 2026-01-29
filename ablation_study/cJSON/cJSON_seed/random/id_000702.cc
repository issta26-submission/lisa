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
//<ID> 702
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *child = (cJSON *)0;
    cJSON *child_ref = (cJSON *)0;
    cJSON *true_item = (cJSON *)0;
    cJSON *false_item = (cJSON *)0;
    cJSON *num_item = (cJSON *)0;
    cJSON *str_item = (cJSON *)0;
    cJSON *retr_child = (cJSON *)0;
    cJSON *retr_child_ref = (cJSON *)0;
    cJSON *retr_true = (cJSON *)0;
    cJSON *retr_false = (cJSON *)0;
    char *printed = (char *)0;
    cJSON_bool add_child_ok = 0;
    cJSON_bool add_ref_ok = 0;
    cJSON_bool add_true_ok = 0;
    cJSON_bool add_false_ok = 0;
    int presence_count = 0;
    cJSON_bool is_child_obj = 0;
    cJSON_bool is_childref_obj = 0;
    cJSON_bool is_true_flag = 0;
    cJSON_bool is_false_flag = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    child = cJSON_CreateObject();
    num_item = cJSON_CreateNumber(42.0);
    str_item = cJSON_CreateString("example");
    true_item = cJSON_CreateTrue();
    false_item = cJSON_CreateFalse();

    // step 3: Configure
    add_child_ok = cJSON_AddItemToObject(child, "value", num_item);
    (void)add_child_ok;
    cJSON_AddItemToObject(child, "label", str_item);
    add_child_ok = cJSON_AddItemToObject(root, "child", child);

    // To avoid potential double-free issues, create a duplicate of `child`
    // and add that duplicate as "childRef" instead of creating a raw object reference.
    child_ref = cJSON_Duplicate(child, 1); // recurse = 1 (true)
    add_ref_ok = cJSON_AddItemToObject(root, "childRef", child_ref);

    add_true_ok = cJSON_AddItemToObject(root, "flagTrue", true_item);
    add_false_ok = cJSON_AddItemToObject(root, "flagFalse", false_item);

    // step 4: Operate
    printed = cJSON_PrintUnformatted(root);
    retr_child = cJSON_GetObjectItem(root, "child");
    retr_child_ref = cJSON_GetObjectItem(root, "childRef");
    retr_true = cJSON_GetObjectItem(root, "flagTrue");
    retr_false = cJSON_GetObjectItem(root, "flagFalse");
    is_child_obj = cJSON_IsObject(retr_child);
    is_childref_obj = cJSON_IsObject(retr_child_ref);
    is_true_flag = cJSON_IsTrue(retr_true);
    is_false_flag = cJSON_IsFalse(retr_false);

    // step 5: Validate
    presence_count = (printed != (char *)0) + (int)is_child_obj + (int)is_childref_obj + (int)is_true_flag + (int)is_false_flag + (int)add_ref_ok + (int)add_true_ok + (int)add_false_ok;
    (void)presence_count;

    // step 6: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}