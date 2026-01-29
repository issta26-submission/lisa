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
//<ID> 1287
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *num_item = NULL;
    cJSON *alias_item = NULL;
    cJSON *null_item = NULL;
    cJSON *tmp = NULL;
    double val_original = 0.0;
    double val_alias = 0.0;
    cJSON_bool ref_added = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    num_item = cJSON_CreateNumber(123.5);
    cJSON_AddItemToObject(root, "original_number", num_item);
    cJSON_AddNumberToObject(root, "direct_number", 7.25);
    null_item = cJSON_AddNullToObject(root, "missing");

    // step 3: Operate / Validate
    ref_added = cJSON_AddItemReferenceToObject(root, "alias_number", num_item);
    tmp = cJSON_GetObjectItem(root, "original_number");
    val_original = cJSON_GetNumberValue(tmp);
    tmp = cJSON_GetObjectItem(root, "alias_number");
    val_alias = cJSON_GetNumberValue(tmp);
    (void)ref_added;
    (void)val_original;
    (void)val_alias;

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}