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
//<ID> 1288
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *created_num = NULL;
    cJSON *ref_target = NULL;
    cJSON *tmp = NULL;
    double val_created = 0.0;
    double val_referenced = 0.0;
    cJSON_bool ref_added = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    created_num = cJSON_CreateNumber(42.0);
    cJSON_AddItemToObject(root, "created_number", created_num);
    cJSON_AddNumberToObject(root, "direct_number", 7.5);
    cJSON_AddNullToObject(root, "nothing");
    ref_target = cJSON_CreateNumber(100.25);
    cJSON_AddItemToObject(root, "original", ref_target);
    ref_added = cJSON_AddItemReferenceToObject(root, "original_ref", ref_target);

    // step 3: Operate / Validate
    tmp = cJSON_GetObjectItem(root, "created_number");
    val_created = cJSON_GetNumberValue(tmp);
    tmp = cJSON_GetObjectItem(root, "original_ref");
    val_referenced = cJSON_GetNumberValue(tmp);
    (void)val_created;
    (void)val_referenced;
    (void)ref_added;

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}