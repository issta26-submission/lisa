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
//<ID> 704
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *true_item = (cJSON *)0;
    cJSON *false_item = (cJSON *)0;
    cJSON *retr_active = (cJSON *)0;
    cJSON *ref_active = (cJSON *)0;
    char *printed = (char *)0;
    cJSON_bool added_true = 0;
    cJSON_bool added_false = 0;
    cJSON_bool added_ref = 0;
    int presence_count = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    true_item = cJSON_CreateTrue();
    false_item = cJSON_CreateFalse();

    // step 3: Configure
    added_true = cJSON_AddItemToObject(root, "active", true_item);
    added_false = cJSON_AddItemToObject(root, "enabled", false_item);
    retr_active = cJSON_GetObjectItem(root, "active");

    // Instead of creating an object-reference (which can lead to lifetime/double-free issues
    // if the referenced node lives in the same tree), duplicate the retrieved item and add
    // the duplicate. This preserves the logical intent (another entry with same value)
    // without causing ownership conflicts on delete.
    if (retr_active != (cJSON *)0) {
        ref_active = cJSON_Duplicate(retr_active, 1); // recurse flag doesn't matter for primitives
        if (ref_active != (cJSON *)0) {
            added_ref = cJSON_AddItemToObject(root, "refActive", ref_active);
        }
    }

    // step 4: Operate
    printed = cJSON_PrintUnformatted(root);

    // step 5: Validate
    presence_count = (printed != (char *)0) + (retr_active != (cJSON *)0) + (cJSON_GetObjectItem(root, "refActive") != (cJSON *)0) + (added_true != 0) + (added_false != 0) + (added_ref != 0);
    (void)presence_count;
    (void)printed;
    (void)added_true;
    (void)added_false;
    (void)added_ref;
    (void)retr_active;

    // step 6: Cleanup
    if (printed != (char *)0) {
        cJSON_free(printed);
    }
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}