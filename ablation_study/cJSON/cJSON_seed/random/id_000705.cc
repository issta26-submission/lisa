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
//<ID> 705
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
    cJSON *ref = (cJSON *)0;
    cJSON *ttrue = (cJSON *)0;
    cJSON *ffalse = (cJSON *)0;
    char *printed = (char *)0;
    cJSON_bool added_child = 0;
    cJSON_bool added_ref = 0;
    cJSON_bool added_true = 0;
    cJSON_bool added_false = 0;
    int presence_count = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    child = cJSON_CreateObject();
    ttrue = cJSON_CreateTrue();
    ffalse = cJSON_CreateFalse();

    // step 3: Configure
    added_true = cJSON_AddItemToObject(child, "enabled", ttrue);
    added_false = cJSON_AddItemToObject(child, "disabled", ffalse);
    added_child = cJSON_AddItemToObject(root, "primary", child);

    // add a reference to the existing child safely using the reference-adding API
    added_ref = cJSON_AddItemReferenceToObject(root, "primary_ref", child);
    // obtain pointer to the reference node that was added
    ref = cJSON_GetObjectItem(root, "primary_ref");

    // step 4: Operate
    printed = cJSON_PrintUnformatted(root);

    // step 5: Validate
    presence_count = (printed != (char *)0) + (root != (cJSON *)0) + (child != (cJSON *)0) + (ref != (cJSON *)0) + added_child + added_ref + added_true + added_false;
    (void)presence_count;

    // step 6: Cleanup
    if (printed) { cJSON_free(printed); }
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}