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
//<ID> 708
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
    char *printed = (char *)0;
    cJSON_bool added_yes = 0;
    cJSON_bool added_no = 0;
    cJSON_bool added_ref = 0;
    int presence_count = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    true_item = cJSON_CreateTrue();
    false_item = cJSON_CreateFalse();

    // step 3: Configure
    added_yes = cJSON_AddItemToObject(root, "yes", true_item);
    added_no  = cJSON_AddItemToObject(root, "no", false_item);

    // Instead of creating a new reference object explicitly (which may be unsafe
    // for non-object item types), use the "AddItemReference" helper to add a
    // reference to the existing true_item under a new key.
    added_ref = cJSON_AddItemReferenceToObject(root, "ref_yes", true_item);

    // step 4: Operate
    if (root) {
        printed = cJSON_PrintUnformatted(root);
    }

    // step 5: Validate
    presence_count = (printed != (char *)0)
                     + (int)added_yes + (int)added_no + (int)added_ref
                     + (int)cJSON_IsTrue(cJSON_GetObjectItem(root, "yes"))
                     + (int)cJSON_IsFalse(cJSON_GetObjectItem(root, "no"));
    (void)presence_count;

    // step 6: Cleanup
    if (printed) {
        cJSON_free(printed);
    }
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}