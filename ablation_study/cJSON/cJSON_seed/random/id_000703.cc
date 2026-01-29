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
//<ID> 703
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = NULL;
    cJSON *child_true = NULL;
    cJSON *child_false = NULL;
    cJSON *ref_to_child = NULL;
    char *printed = NULL;
    int presence_count = 0;
    cJSON_bool is_true_flag = 0;
    cJSON_bool is_false_flag = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    child_true = cJSON_CreateTrue();
    child_false = cJSON_CreateFalse();

    // step 3: Configure
    cJSON_AddItemToObject(root, "truth", child_true);
    cJSON_AddItemToObject(root, "lie", child_false);

    // create a reference to the existing child_true in a safe way
    if (cJSON_AddItemReferenceToObject(root, "truth_ref", child_true)) {
        ref_to_child = cJSON_GetObjectItem(root, "truth_ref");
    } else {
        // fallback: create an explicit object-reference node and add it
        ref_to_child = cJSON_CreateObjectReference(child_true);
        cJSON_AddItemToObject(root, "truth_ref", ref_to_child);
    }

    // step 4: Operate
    printed = cJSON_PrintUnformatted(root);
    is_true_flag = cJSON_IsTrue(child_true);
    is_false_flag = cJSON_IsFalse(child_false);

    // step 5: Validate (keep presence_count usage as in original logic)
    presence_count = (printed != NULL) + (ref_to_child != NULL) + (int)is_true_flag + (int)is_false_flag;
    (void)presence_count;

    // step 6: Cleanup
    if (printed) {
        cJSON_free(printed);
    }
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}