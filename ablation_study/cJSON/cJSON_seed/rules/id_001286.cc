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
//<ID> 1286
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *num_orig = NULL;
    cJSON *child = NULL;
    cJSON *null_item = NULL;
    cJSON *tmp = NULL;
    double val_original = 0.0;
    double val_ref = 0.0;
    double aggregated = 0.0;
    cJSON_bool add_orig_result = 0;
    cJSON_bool add_child_result = 0;
    cJSON_bool add_ref_result = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    num_orig = cJSON_CreateNumber(13.37);
    add_orig_result = cJSON_AddItemToObject(root, "original_number", num_orig);
    null_item = cJSON_AddNullToObject(root, "nothing_here");
    child = cJSON_CreateObject();
    add_child_result = cJSON_AddItemToObject(root, "child", child);
    add_ref_result = cJSON_AddItemReferenceToObject(child, "original_ref", num_orig);

    // step 3: Operate / Validate
    tmp = cJSON_GetObjectItem(root, "original_number");
    val_original = cJSON_GetNumberValue(tmp);
    tmp = cJSON_GetObjectItem(child, "original_ref");
    val_ref = cJSON_GetNumberValue(tmp);
    aggregated = val_original + val_ref;
    (void)aggregated;
    (void)add_orig_result;
    (void)add_child_result;
    (void)add_ref_result;
    (void)val_original;
    (void)val_ref;

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}